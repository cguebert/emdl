#include <emdl/dul/StateMachine.h>

#include <cstdint>
#include <functional>
#include <map>
#include <sstream>
#include <tuple>
#include <utility>

#include <boost/asio.hpp>
#include <boost/system/system_error.hpp>

#include <emdl/dul/EventData.h>
#include <emdl/dul/Transport.h>

#include "odil/AssociationParameters.h"
#include "odil/endian.h"
#include "odil/Exception.h"
#include "odil/pdu/AAbort.h"
#include "odil/pdu/AAssociate.h"
#include "odil/pdu/AAssociateRJ.h"
#include "odil/pdu/AReleaseRP.h"
#include "odil/pdu/AReleaseRQ.h"
#include "odil/pdu/PDataTF.h"

namespace emdl
{
	namespace dul
	{
		StateMachine::StateMachine()
			: m_artimTimer(m_transport.service())
			, m_associationAcceptor(odil::default_association_acceptor)
		{
			setState(StateId::Sta1);
		}

		void StateMachine::transition(Event event, EventData& data)
		{
			const auto& transitions = m_currentState->transitions;
			const auto it = std::find_if(transitions.begin(), transitions.end(), [event](const Transition t) {
				return t.event == event;
			});
			if (it == transitions.end())
				throw Exception("No such transition");

			setState(it->nextState); // Set it before launching the action so some actions can change it again

			// Do action
			const auto actionIndex = static_cast<size_t>(it->action);
			(this->*m_actions[actionIndex])(data);
		}

		StateMachine::StateId StateMachine::state() const
		{
			return m_currentState->id;
		}

		const Transport& StateMachine::transport() const
		{
			return m_transport;
		}

		Transport& StateMachine::transport()
		{
			return m_transport;
		}

		StateMachine::duration_type StateMachine::timeout() const
		{
			return m_timeout;
		}

		void StateMachine::setTimeout(duration_type timeout)
		{
			m_timeout = timeout;
		}

		void StateMachine::setSocket(std::shared_ptr<Transport::Socket> socket)
		{
			m_transport.setSocket(socket);
			EventData data;
			transition(Event::TransportConnectionIndication, data);
		}

		void StateMachine::sendPdu(EventData& data)
		{
			if (!data.pdu)
				throw Exception("No PDU");

			const auto& item = data.pdu->get_item();
			const auto type = item.as_unsigned_int_8("PDU-type");

			switch (type)
			{
			case 0x01:
				transition(Event::AAssociateRQLocal, data);
				transition(Event::TransportConnectionConfirmation, data);
				break;
			case 0x02:
				transition(Event::AAssociateACLocal, data);
				break;
			case 0x03:
				transition(Event::AAssociateRJLocal, data);
				break;
			case 0x04:
				transition(Event::PDataTFLocal, data);
				break;
			case 0x05:
				transition(Event::AReleaseRQLocal, data);
				break;
			case 0x06:
				transition(Event::AReleaseRPLocal, data);
				break;
			case 0x07:
				transition(Event::AAbortLocal, data);
				break;
			default:
				transition(Event::InvalidPDU, data);
			}
		}

		void StateMachine::receivePdu(EventData& data)
		{
			const auto header = m_transport.read(6);

			const uint8_t type = header[0];
			const uint32_t length = odil::big_endian_to_host(*reinterpret_cast<const uint32_t*>(&header[0] + 2));
			const auto pdu_data = m_transport.read(length);

			std::stringstream stream;
			stream.write(&header[0], header.size());
			stream.write(&pdu_data[0], pdu_data.size());

			data.pdu = nullptr;
			Event event = Event::None;
			switch (type)
			{
			case 0x01:
				data.pdu = std::make_shared<odil::pdu::AAssociateRQ>(stream);
				event = Event::AAssociateRQRemote;
				break;
			case 0x02:
				data.pdu = std::make_shared<odil::pdu::AAssociateAC>(stream);
				event = Event::AAssociateACRemote;
				break;
			case 0x03:
				data.pdu = std::make_shared<odil::pdu::AAssociateRJ>(stream);
				event = Event::AAssociateRJRemote;
				break;
			case 0x04:
				data.pdu = std::make_shared<odil::pdu::PDataTF>(stream);
				event = Event::PDataTFRemote;
				break;
			case 0x05:
				data.pdu = std::make_shared<odil::pdu::AReleaseRQ>(stream);
				event = Event::AReleaseRQRemote;
				break;
			case 0x06:
				data.pdu = std::make_shared<odil::pdu::AReleaseRP>(stream);
				event = Event::AReleaseRPRemote;
				break;
			case 0x07:
				data.pdu = std::make_shared<odil::pdu::AAbort>(stream);
				event = Event::AAbortRemote;
				break;
			default:
				event = Event::InvalidPDU;
			}

			transition(event, data);
		}

		void StateMachine::startTimer(EventData& data)
		{
			return;

			if (m_artimTimer.expires_from_now(m_timeout))
				throw Exception("ARTIM timer started with pending operations");

			m_artimTimer.async_wait(
				[this, &data](const boost::system::error_code& e) {
					if (!e)
						transition(Event::ARTIMTimerExpired, data);
					else if (e != boost::asio::error::operation_aborted)
						throw boost::system::system_error(e);
				});
		}

		void StateMachine::stopTimer()
		{
			return;

			m_artimTimer.expires_at(boost::posix_time::pos_infin);
			m_transport.service().poll();
			// FIXME: check that the timer was aborted
			/*
		if(source != Source::TIMER)
		{
			throw Exception("Unknown event");
		}
		else if(error != boost::asio::error::operation_aborted)
		{
			throw Exception("TCP timer error: "+error.message());
		}
	*/
			m_transport.service().reset();
		}

		const odil::AssociationAcceptor& StateMachine::associationAcceptor() const
		{
			return m_associationAcceptor;
		}

		void StateMachine::setAssociationAcceptor(const odil::AssociationAcceptor& acceptor)
		{
			m_associationAcceptor = acceptor;
		}

#define tr_def(event, action, end)                                                           \
	{                                                                                        \
		StateMachine::Event::event, StateMachine::Action::action, StateMachine::StateId::end \
	}

		// clang-format off
		const StateMachine::States StateMachine::m_states = {
			{ StateId::Sta1, {
				tr_def(AAssociateRQLocal, AE_1, Sta4),
				tr_def(TransportConnectionIndication, AE_5, Sta2)
			} },
			{ StateId::Sta2, {
				tr_def(AAssociateACRemote, AA_1, Sta13),
				tr_def(AAssociateRJRemote, AA_1, Sta13),
				tr_def(AAssociateRQRemote, AE_6, Sta3), // Inside AE_6 we can choose to go to Sta13
				tr_def(PDataTFRemote, AA_1, Sta13),
				tr_def(AReleaseRQRemote, AA_1, Sta13),
				tr_def(AReleaseRPRemote, AA_1, Sta13),
				tr_def(AAbortRemote, AA_2, Sta1),
				tr_def(TransportConnectionClosedIndication, AA_5, Sta1),
				tr_def(ARTIMTimerExpired, AA_2, Sta1),
				tr_def(InvalidPDU, AA_1, Sta13)
			} },
			{ StateId::Sta3, {
				tr_def(AAssociateACRemote, AA_8, Sta13),
				tr_def(AAssociateRJRemote, AA_8, Sta13),
				tr_def(AAssociateRQRemote, AA_8, Sta13),
				tr_def(AAssociateACLocal, AE_7, Sta6),
				tr_def(AAssociateRJLocal, AE_8, Sta13),
				tr_def(PDataTFRemote, AA_8, Sta13),
				tr_def(AReleaseRQRemote, AA_8, Sta13),
				tr_def(AReleaseRPRemote, AA_8, Sta13),
				tr_def(AAbortLocal, AA_1, Sta13),
				tr_def(AAbortRemote, AA_3, Sta1),
				tr_def(TransportConnectionClosedIndication, AA_4, Sta1),
				tr_def(InvalidPDU, AA_8, Sta13)
			} },
			{ StateId::Sta4, {
				tr_def(TransportConnectionConfirmation, AE_2, Sta5),
				tr_def(AAbortLocal, AA_2, Sta1),
				tr_def(TransportConnectionClosedIndication, AA_4, Sta1)
			} },
			{ StateId::Sta5, {
				tr_def(AAssociateACRemote, AE_3, Sta6),
				tr_def(AAssociateRJRemote, AE_4, Sta1),
				tr_def(AAssociateRQRemote, AA_8, Sta13),
				tr_def(PDataTFRemote, AA_8, Sta13),
				tr_def(AReleaseRQRemote, AA_8, Sta13),
				tr_def(AReleaseRPRemote, AA_8, Sta13),
				tr_def(AAbortLocal, AA_1, Sta13),
				tr_def(AAbortRemote, AA_3, Sta1),
				tr_def(TransportConnectionClosedIndication, AA_4, Sta1),
				tr_def(InvalidPDU, AA_8, Sta13)
			} },
			{ StateId::Sta6, {
				tr_def(AAssociateACRemote, AA_8, Sta13),
				tr_def(AAssociateRJRemote, AA_8, Sta13),
				tr_def(AAssociateRQRemote, AA_8, Sta13),
				tr_def(PDataTFLocal, DT_1, Sta6),
				tr_def(PDataTFRemote, DT_2, Sta6),
				tr_def(AReleaseRQLocal, AR_1, Sta7),
				tr_def(AReleaseRQRemote, AR_2, Sta8),
				tr_def(AReleaseRPRemote, AA_8, Sta13),
				tr_def(AAbortLocal, AA_1, Sta13),
				tr_def(AAbortRemote, AA_3, Sta1),
				tr_def(TransportConnectionClosedIndication, AA_4, Sta1),
				tr_def(InvalidPDU, AA_8, Sta13)
			} },
			{ StateId::Sta7, {
				tr_def(AAssociateACRemote, AA_8, Sta13),
				tr_def(AAssociateRJRemote, AA_8, Sta13),
				tr_def(AAssociateRQRemote, AA_8, Sta13),
				tr_def(PDataTFRemote, AR_6, Sta7),
			//	tr_def(AReleaseRQRemote, AR_8, Sta9Or10),
				tr_def(AReleaseRPRemote, AR_3, Sta1),
				tr_def(AAbortLocal, AA_1, Sta13),
				tr_def(AAbortRemote, AA_3, Sta1),
				tr_def(TransportConnectionClosedIndication, AA_4, Sta1),
				tr_def(InvalidPDU, AA_8, Sta13)
			} },
			{ StateId::Sta8, {
				tr_def(AAssociateACRemote, AA_8, Sta13),
				tr_def(AAssociateRJRemote, AA_8, Sta13),
				tr_def(AAssociateRQRemote, AA_8, Sta13),
				tr_def(PDataTFLocal, AR_7, Sta8),
				tr_def(PDataTFRemote, AA_8, Sta13),
				tr_def(AReleaseRQRemote, AA_8, Sta13),
				tr_def(AReleaseRPRemote, AA_8, Sta13),
				tr_def(AReleaseRPLocal, AR_4, Sta13),
				tr_def(AAbortLocal, AA_1, Sta13),
				tr_def(AAbortRemote, AA_3, Sta1),
				tr_def(TransportConnectionClosedIndication, AA_4, Sta1),
				tr_def(InvalidPDU, AA_8, Sta13)
			} },
			{ StateId::Sta9, {
				tr_def(AAssociateACRemote, AA_8, Sta13),
				tr_def(AAssociateRJRemote, AA_8, Sta13),
				tr_def(AAssociateRQRemote, AA_8, Sta13),
				tr_def(PDataTFRemote, AA_8, Sta13),
				tr_def(AReleaseRQRemote, AA_8, Sta13),
				tr_def(AReleaseRPRemote, AA_8, Sta13),
				tr_def(AReleaseRPLocal, AR_9, Sta11),
				tr_def(AAbortLocal, AA_1, Sta13),
				tr_def(AAbortRemote, AA_3, Sta1),
				tr_def(TransportConnectionClosedIndication, AA_4, Sta1),
				tr_def(InvalidPDU, AA_8, Sta13)
			} },
			{ StateId::Sta10, {
				tr_def(AAssociateACRemote, AA_8, Sta13),
				tr_def(AAssociateRJRemote, AA_8, Sta13),
				tr_def(AAssociateRQRemote, AA_8, Sta13),
				tr_def(PDataTFRemote, AA_8, Sta13),
				tr_def(AReleaseRQRemote, AA_8, Sta13),
				tr_def(AReleaseRPRemote, AR_10, Sta12),
				tr_def(AAbortLocal, AA_1, Sta13),
				tr_def(AAbortRemote, AA_3, Sta1),
				tr_def(TransportConnectionClosedIndication, AA_4, Sta1),
				tr_def(InvalidPDU, AA_8, Sta13)
			} },
			{ StateId::Sta11, {
				tr_def(AAssociateACRemote, AA_8, Sta13),
				tr_def(AAssociateRJRemote, AA_8, Sta13),
				tr_def(AAssociateRQRemote, AA_8, Sta13),
				tr_def(PDataTFRemote, AA_8, Sta13),
				tr_def(AReleaseRQRemote, AA_8, Sta13),
				tr_def(AReleaseRPRemote, AR_3, Sta1),
				tr_def(AAbortLocal, AA_1, Sta13),
				tr_def(AAbortRemote, AA_3, Sta1),
				tr_def(TransportConnectionClosedIndication, AA_4, Sta1),
				tr_def(InvalidPDU, AA_8, Sta13)
			} },
			{ StateId::Sta12, {
				tr_def(AAssociateACRemote, AA_8, Sta13),
				tr_def(AAssociateRJRemote, AA_8, Sta13),
				tr_def(AAssociateRQRemote, AA_8, Sta13),
				tr_def(PDataTFRemote, AA_8, Sta13),
				tr_def(AReleaseRQRemote, AA_8, Sta13),
				tr_def(AReleaseRPRemote, AA_8, Sta13),
				tr_def(AReleaseRPLocal, AR_4, Sta13),
				tr_def(AAbortLocal, AA_1, Sta13),
				tr_def(AAbortRemote, AA_3, Sta1),
				tr_def(TransportConnectionClosedIndication, AA_4, Sta1),
				tr_def(InvalidPDU, AA_8, Sta13)
			} },
			{ StateId::Sta13, {
				tr_def(AAssociateACRemote, AA_6, Sta13),
				tr_def(AAssociateRJRemote, AA_6, Sta13),
				tr_def(AAssociateRQRemote, AA_7, Sta13),
				tr_def(PDataTFRemote, AA_6, Sta13),
				tr_def(AReleaseRQRemote, AA_6, Sta13),
				tr_def(AReleaseRPRemote, AA_6, Sta13),
				tr_def(AAbortRemote, AA_2, Sta1),
				tr_def(TransportConnectionClosedIndication, AR_5, Sta1),
				tr_def(ARTIMTimerExpired, AA_2, Sta1),
				tr_def(InvalidPDU, AA_7, Sta13)
			} },
		}; // clang-format on
#undef tr_def

		const StateMachine::ActionList StateMachine::m_actions = {
			&StateMachine::AE_1,
			&StateMachine::AE_2,
			&StateMachine::AE_3,
			&StateMachine::AE_4,
			&StateMachine::AE_5,
			&StateMachine::AE_6,
			&StateMachine::AE_7,
			&StateMachine::AE_8,

			&StateMachine::DT_1,
			&StateMachine::DT_2,

			&StateMachine::AR_1,
			&StateMachine::AR_2,
			&StateMachine::AR_3,
			&StateMachine::AR_4,
			&StateMachine::AR_5,
			&StateMachine::AR_6,
			&StateMachine::AR_7,
			&StateMachine::AR_8,
			&StateMachine::AR_9,
			&StateMachine::AR_10,

			&StateMachine::AA_1,
			&StateMachine::AA_2,
			&StateMachine::AA_3,
			&StateMachine::AA_4,
			&StateMachine::AA_5,
			&StateMachine::AA_6,
			&StateMachine::AA_7,
			&StateMachine::AA_8};

		void StateMachine::setState(StateId state)
		{
			m_currentState = &m_states[static_cast<size_t>(state)];
		}

		void StateMachine::sendPdu(EventData& data, uint8_t pdu_type)
		{
			if (!data.pdu)
				throw Exception("No PDU");

			const auto& item = data.pdu->get_item();

			if (item.as_unsigned_int_8("PDU-type") != pdu_type)
				throw Exception("Invalid PDU");

			std::ostringstream stream;
			stream << item;
			m_transport.write(stream.str());
		}

		void StateMachine::AE_1(EventData& data)
		{
			m_transport.connect(data.peer_endpoint);
		}

		void StateMachine::AE_2(EventData& data)
		{
			sendPdu(data, 0x01);
		}

		void StateMachine::AE_3(EventData&)
		{
			// Do nothing: notification is implicit since this function is only called by receivePdu
		}

		void StateMachine::AE_4(EventData&)
		{
			// Notification is implicit since this function is only called by receivePdu
			m_transport.close();
		}

		void StateMachine::AE_5(EventData& data)
		{
			// Connection response has already been sent.
			startTimer(data);
		}

		void StateMachine::AE_6(EventData& data)
		{
			stopTimer();

			try
			{
				const odil::AssociationParameters input_parameters(
					*std::dynamic_pointer_cast<odil::pdu::AAssociateRQ>(data.pdu));
				data.association_parameters = associationAcceptor()(input_parameters);
				// Issue A-ASSOCIATE indication
				// Do nothing: notification is implicit since this function is only called by receivePdu
				setState(StateId::Sta3);
			}
			catch (const odil::AssociationRejected& reject)
			{
				data.pdu = std::make_shared<odil::pdu::AAssociateRJ>(reject.get_result(), reject.get_source(), reject.get_reason());
				startTimer(data);
				sendPdu(data, 0x03);
				data.pdu = nullptr;
				setState(StateId::Sta13);
			}
		}

		void StateMachine::AE_7(EventData& data)
		{
			sendPdu(data, 0x02);
		}

		void StateMachine::AE_8(EventData& data)
		{
			sendPdu(data, 0x03);
			startTimer(data);
		}

		void StateMachine::DT_1(EventData& data)
		{
			sendPdu(data, 0x04);
		}

		void StateMachine::DT_2(EventData&)
		{
			// Do nothing: notification is implicit since this function is only called by receivePdu
		}

		void StateMachine::AR_1(EventData& data)
		{
			sendPdu(data, 0x05);
		}

		void StateMachine::AR_2(EventData&)
		{
			// Do nothing: notification is implicit since this function is only called by receivePdu
		}

		void StateMachine::AR_3(EventData&)
		{
			// Notification is implicit since this function is only called by receivePdu
			m_transport.close();
		}

		void StateMachine::AR_4(EventData& data)
		{
			sendPdu(data, 0x06);
			startTimer(data);
		}

		void StateMachine::AR_5(EventData&)
		{
			stopTimer();
		}

		void StateMachine::AR_6(EventData&)
		{
			// Do nothing: notification is implicit since this function is only called by receivePdu
		}

		void StateMachine::AR_7(EventData& data)
		{
			sendPdu(data, 0x04);
		}

		void StateMachine::AR_8(EventData&)
		{
			// Do nothing: notification is implicit since this function is only called by receivePdu
		}

		void StateMachine::AR_9(EventData& data)
		{
			sendPdu(data, 0x06);
		}

		void StateMachine::AR_10(EventData&)
		{
			// Do nothing: notification is implicit since this function is only called by receivePdu
		}

		void StateMachine::AA_1(EventData& data)
		{
			if (std::dynamic_pointer_cast<odil::pdu::AAbort>(data.pdu))
			{
				sendPdu(data, 0x07);
			}
			else
			{
				data.pdu = std::make_shared<odil::pdu::AAbort>(1, 2);
				sendPdu(data);
			}

			startTimer(data);
		}

		void StateMachine::AA_2(EventData&)
		{
			stopTimer();
			m_transport.close();
		}

		void StateMachine::AA_3(EventData&)
		{
			// Notification is implicit since this function is only called by receivePdu
			m_transport.close();
		}

		void StateMachine::AA_4(EventData&)
		{
			// Do nothing: notification is implicit since this function is only called by receivePdu
		}

		void StateMachine::AA_5(EventData&)
		{
			stopTimer();
		}

		void StateMachine::AA_6(EventData&)
		{
			// Nothing to do.
		}

		void StateMachine::AA_7(EventData& data)
		{
			sendPdu(data, 0x07);
		}

		void StateMachine::AA_8(EventData& data)
		{
			data.pdu = std::make_shared<odil::pdu::AAbort>(2, 2);
			sendPdu(data, 0x07);
			// Notification is implicit
			startTimer(data);
		}
	}
}
