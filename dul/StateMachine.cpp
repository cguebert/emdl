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
			: m_state(State::Sta1)
			, m_timeout(boost::posix_time::pos_infin)
			, m_artim_timer(m_transport.get_service())
			, m_association_acceptor(odil::default_association_acceptor)
		{
		}

		void StateMachine::transition(Event const& event, EventData& data)
		{
			const auto guard_iterator = StateMachine::m_guards.find(
				{m_state, event});
			const auto guard_value = (guard_iterator != StateMachine::m_guards.end()) ? guard_iterator->second(*this, data) : true;

			const auto transition_iterator = StateMachine::m_transitions.find(
				std::make_tuple(m_state, event, guard_value));
			if (transition_iterator == StateMachine::m_transitions.end())
			{
				throw Exception("No such transition");
			}

			const auto& action = transition_iterator->second.first;
			const auto& next_state = transition_iterator->second.second;

			// Do action
			const auto action_index = static_cast<size_t>(action);
			if (action_index > m_actions.size())
			{
				throw Exception("Unknown action");
			}
			(this->*m_actions[action_index])(data);

			m_state = next_state;
		}

		StateMachine::State StateMachine::get_state() const
		{
			return m_state;
		}

		const Transport& StateMachine::get_transport() const
		{
			return m_transport;
		}

		Transport& StateMachine::get_transport()
		{
			return m_transport;
		}

		StateMachine::duration_type StateMachine::get_timeout() const
		{
			return m_timeout;
		}

		void StateMachine::set_timeout(duration_type timeout)
		{
			m_timeout = timeout;
		}

		void StateMachine::receive(EventData& data)
		{
			m_transport.receive(data.socket);
			transition(Event::TransportConnectionIndication, data);
		}

		void StateMachine::send_pdu(EventData& data)
		{
			if (data.pdu == nullptr)
			{
				throw Exception("No PDU");
			}
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

		void StateMachine::receive_pdu(EventData& data)
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

		void StateMachine::start_timer(EventData& data)
		{
			return;

			const auto canceled = m_artim_timer.expires_from_now(m_timeout);
			if (canceled != 0)
			{
				throw Exception("ARTIM timer started with pending operations");
			}

			m_artim_timer.async_wait(
				[this, &data](const boost::system::error_code& e) {
					//source = Source::TIMER;
					//error = e;

					if (!e)
					{
						transition(Event::ARTIMTimerExpired, data);
					}
					else if (e == boost::asio::error::operation_aborted)
					{
						// Do nothing
					}
					else
					{
						throw boost::system::system_error(e);
					}
				});
		}

		void StateMachine::stop_timer()
		{
			return;

			m_artim_timer.expires_at(boost::posix_time::pos_infin);
			m_transport.get_service().poll();
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
			m_transport.get_service().reset();
		}

		const odil::AssociationAcceptor& StateMachine::get_association_acceptor() const
		{
			return m_association_acceptor;
		}

		void StateMachine::set_association_acceptor(const odil::AssociationAcceptor& acceptor)
		{
			m_association_acceptor = acceptor;
		}

#define transition_full(start, event, guard, action, end)                               \
	{                                                                                   \
		std::make_tuple(StateMachine::State::start, StateMachine::Event::event, guard), \
		{                                                                               \
			StateMachine::Action::action, StateMachine::State::end                      \
		}                                                                               \
	}

#define transition_s(start, event, action, end) \
	transition_full(start, event, true, action, end)

		const StateMachine::TransitionMap StateMachine::m_transitions = {
			transition_s(Sta1, AAssociateRQLocal, AE_1, Sta4),
			transition_s(Sta1, TransportConnectionIndication, AE_5, Sta2),

			transition_s(Sta2, AAssociateACRemote, AA_1, Sta13),
			transition_s(Sta2, AAssociateRJRemote, AA_1, Sta13),
			transition_full(Sta2, AAssociateRQRemote, true, AE_6, Sta3),
			transition_full(Sta2, AAssociateRQRemote, false, AE_6, Sta13),
			transition_s(Sta2, PDataTFRemote, AA_1, Sta13),
			transition_s(Sta2, AReleaseRQRemote, AA_1, Sta13),
			transition_s(Sta2, AReleaseRPRemote, AA_1, Sta13),
			transition_s(Sta2, AAbortRemote, AA_2, Sta1),
			transition_s(Sta2, TransportConnectionClosedIndication, AA_5, Sta1),
			transition_s(Sta2, ARTIMTimerExpired, AA_2, Sta1),
			transition_s(Sta2, InvalidPDU, AA_1, Sta13),

			transition_s(Sta3, AAssociateACRemote, AA_8, Sta13),
			transition_s(Sta3, AAssociateRJRemote, AA_8, Sta13),
			transition_s(Sta3, AAssociateRQRemote, AA_8, Sta13),
			transition_s(Sta3, AAssociateACLocal, AE_7, Sta6),
			transition_s(Sta3, AAssociateRJLocal, AE_8, Sta13),
			transition_s(Sta3, PDataTFRemote, AA_8, Sta13),
			transition_s(Sta3, AReleaseRQRemote, AA_8, Sta13),
			transition_s(Sta3, AReleaseRPRemote, AA_8, Sta13),
			transition_s(Sta3, AAbortLocal, AA_1, Sta13),
			transition_s(Sta3, AAbortRemote, AA_3, Sta1),
			transition_s(Sta3, TransportConnectionClosedIndication, AA_4, Sta1),
			transition_s(Sta3, InvalidPDU, AA_8, Sta13),

			transition_s(Sta4, TransportConnectionConfirmation, AE_2, Sta5),
			transition_s(Sta4, AAbortLocal, AA_2, Sta1),
			transition_s(Sta4, TransportConnectionClosedIndication, AA_4, Sta1),

			transition_s(Sta5, AAssociateACRemote, AE_3, Sta6),
			transition_s(Sta5, AAssociateRJRemote, AE_4, Sta1),
			transition_s(Sta5, AAssociateRQRemote, AA_8, Sta13),
			transition_s(Sta5, PDataTFRemote, AA_8, Sta13),
			transition_s(Sta5, AReleaseRQRemote, AA_8, Sta13),
			transition_s(Sta5, AReleaseRPRemote, AA_8, Sta13),
			transition_s(Sta5, AAbortLocal, AA_1, Sta13),
			transition_s(Sta5, AAbortRemote, AA_3, Sta1),
			transition_s(Sta5, TransportConnectionClosedIndication, AA_4, Sta1),
			transition_s(Sta5, InvalidPDU, AA_8, Sta13),

			transition_s(Sta6, AAssociateACRemote, AA_8, Sta13),
			transition_s(Sta6, AAssociateRJRemote, AA_8, Sta13),
			transition_s(Sta6, AAssociateRQRemote, AA_8, Sta13),
			transition_s(Sta6, PDataTFLocal, DT_1, Sta6),
			transition_s(Sta6, PDataTFRemote, DT_2, Sta6),
			transition_s(Sta6, AReleaseRQLocal, AR_1, Sta7),
			transition_s(Sta6, AReleaseRQRemote, AR_2, Sta8),
			transition_s(Sta6, AReleaseRPRemote, AA_8, Sta13),
			transition_s(Sta6, AAbortLocal, AA_1, Sta13),
			transition_s(Sta6, AAbortRemote, AA_3, Sta1),
			transition_s(Sta6, TransportConnectionClosedIndication, AA_4, Sta1),
			transition_s(Sta6, InvalidPDU, AA_8, Sta13),

			transition_s(Sta7, AAssociateACRemote, AA_8, Sta13),
			transition_s(Sta7, AAssociateRJRemote, AA_8, Sta13),
			transition_s(Sta7, AAssociateRQRemote, AA_8, Sta13),
			transition_s(Sta7, PDataTFRemote, AR_6, Sta7),
			//transition(Sta7, AReleaseRQRemote, AR_8, Sta9Or10),
			transition_s(Sta7, AReleaseRPRemote, AR_3, Sta1),
			transition_s(Sta7, AAbortLocal, AA_1, Sta13),
			transition_s(Sta7, AAbortRemote, AA_3, Sta1),
			transition_s(Sta7, TransportConnectionClosedIndication, AA_4, Sta1),
			transition_s(Sta7, InvalidPDU, AA_8, Sta13),

			transition_s(Sta8, AAssociateACRemote, AA_8, Sta13),
			transition_s(Sta8, AAssociateRJRemote, AA_8, Sta13),
			transition_s(Sta8, AAssociateRQRemote, AA_8, Sta13),
			transition_s(Sta8, PDataTFLocal, AR_7, Sta8),
			transition_s(Sta8, PDataTFRemote, AA_8, Sta13),
			transition_s(Sta8, AReleaseRQRemote, AA_8, Sta13),
			transition_s(Sta8, AReleaseRPRemote, AA_8, Sta13),
			transition_s(Sta8, AReleaseRPLocal, AR_4, Sta13),
			transition_s(Sta8, AAbortLocal, AA_1, Sta13),
			transition_s(Sta8, AAbortRemote, AA_3, Sta1),
			transition_s(Sta8, TransportConnectionClosedIndication, AA_4, Sta1),
			transition_s(Sta8, InvalidPDU, AA_8, Sta13),

			transition_s(Sta9, AAssociateACRemote, AA_8, Sta13),
			transition_s(Sta9, AAssociateRJRemote, AA_8, Sta13),
			transition_s(Sta9, AAssociateRQRemote, AA_8, Sta13),
			transition_s(Sta9, PDataTFRemote, AA_8, Sta13),
			transition_s(Sta9, AReleaseRQRemote, AA_8, Sta13),
			transition_s(Sta9, AReleaseRPRemote, AA_8, Sta13),
			transition_s(Sta9, AReleaseRPLocal, AR_9, Sta11),
			transition_s(Sta9, AAbortLocal, AA_1, Sta13),
			transition_s(Sta9, AAbortRemote, AA_3, Sta1),
			transition_s(Sta9, TransportConnectionClosedIndication, AA_4, Sta1),
			transition_s(Sta9, InvalidPDU, AA_8, Sta13),

			transition_s(Sta10, AAssociateACRemote, AA_8, Sta13),
			transition_s(Sta10, AAssociateRJRemote, AA_8, Sta13),
			transition_s(Sta10, AAssociateRQRemote, AA_8, Sta13),
			transition_s(Sta10, PDataTFRemote, AA_8, Sta13),
			transition_s(Sta10, AReleaseRQRemote, AA_8, Sta13),
			transition_s(Sta10, AReleaseRPRemote, AR_10, Sta12),
			transition_s(Sta10, AAbortLocal, AA_1, Sta13),
			transition_s(Sta10, AAbortRemote, AA_3, Sta1),
			transition_s(Sta10, TransportConnectionClosedIndication, AA_4, Sta1),
			transition_s(Sta10, InvalidPDU, AA_8, Sta13),

			transition_s(Sta11, AAssociateACRemote, AA_8, Sta13),
			transition_s(Sta11, AAssociateRJRemote, AA_8, Sta13),
			transition_s(Sta11, AAssociateRQRemote, AA_8, Sta13),
			transition_s(Sta11, PDataTFRemote, AA_8, Sta13),
			transition_s(Sta11, AReleaseRQRemote, AA_8, Sta13),
			transition_s(Sta11, AReleaseRPRemote, AR_3, Sta1),
			transition_s(Sta11, AAbortLocal, AA_1, Sta13),
			transition_s(Sta11, AAbortRemote, AA_3, Sta1),
			transition_s(Sta11, TransportConnectionClosedIndication, AA_4, Sta1),
			transition_s(Sta11, InvalidPDU, AA_8, Sta13),

			transition_s(Sta12, AAssociateACRemote, AA_8, Sta13),
			transition_s(Sta12, AAssociateRJRemote, AA_8, Sta13),
			transition_s(Sta12, AAssociateRQRemote, AA_8, Sta13),
			transition_s(Sta12, PDataTFRemote, AA_8, Sta13),
			transition_s(Sta12, AReleaseRQRemote, AA_8, Sta13),
			transition_s(Sta12, AReleaseRPRemote, AA_8, Sta13),
			transition_s(Sta12, AReleaseRPLocal, AR_4, Sta13),
			transition_s(Sta12, AAbortLocal, AA_1, Sta13),
			transition_s(Sta12, AAbortRemote, AA_3, Sta1),
			transition_s(Sta12, TransportConnectionClosedIndication, AA_4, Sta1),
			transition_s(Sta12, InvalidPDU, AA_8, Sta13),

			transition_s(Sta13, AAssociateACRemote, AA_6, Sta13),
			transition_s(Sta13, AAssociateRJRemote, AA_6, Sta13),
			transition_s(Sta13, AAssociateRQRemote, AA_7, Sta13),
			transition_s(Sta13, PDataTFRemote, AA_6, Sta13),
			transition_s(Sta13, AReleaseRQRemote, AA_6, Sta13),
			transition_s(Sta13, AReleaseRPRemote, AA_6, Sta13),
			transition_s(Sta13, AAbortRemote, AA_2, Sta1),
			transition_s(Sta13, TransportConnectionClosedIndication, AR_5, Sta1),
			transition_s(Sta13, ARTIMTimerExpired, AA_2, Sta1),
			transition_s(Sta13, InvalidPDU, AA_7, Sta13),
		};

#undef transition
#undef transition_full

		const StateMachine::GuardMap StateMachine::m_guards = {
			{{StateMachine::State::Sta2, StateMachine::Event::AAssociateRQRemote},
			 [](const StateMachine& state_machine, EventData& data) {
				 try
				 {
					 const odil::AssociationParameters input_parameters(
						 *std::dynamic_pointer_cast<odil::pdu::AAssociateRQ>(data.pdu));
					 data.association_parameters = state_machine.get_association_acceptor()(input_parameters);
				 }
				 catch (const odil::AssociationRejected& reject)
				 {
					 data.reject = std::make_shared<odil::AssociationRejected>(reject);
					 return false;
				 }
				 return true;
			 }},
		};

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

		void StateMachine::send_pdu(EventData& data, uint8_t pdu_type)
		{
			if (data.pdu == nullptr)
			{
				throw Exception("No PDU");
			}
			const auto& item = data.pdu->get_item();

			if (item.as_unsigned_int_8("PDU-type") != pdu_type)
			{
				throw Exception("Invalid PDU");
			}

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
			send_pdu(data, 0x01);
		}

		void StateMachine::AE_3(EventData&)
		{
			// Do nothing: notification is implicit since this function is only called
			// by receive_pdu
		}

		void StateMachine::AE_4(EventData&)
		{
			// Notification is implicit since this function is only called by
			// receive_pdu
			m_transport.close();
		}

		void StateMachine::AE_5(EventData& data)
		{
			// Connection response has already been sent.
			start_timer(data);
		}

		void StateMachine::AE_6(EventData& data)
		{
			stop_timer();

			if (data.reject)
			{
				data.pdu = std::make_shared<odil::pdu::AAssociateRJ>(
					data.reject->get_result(), data.reject->get_source(),
					data.reject->get_reason());
				send_pdu(data, 0x03);
				data.pdu = NULL;
			}
			else
			{
				// Issue A-ASSOCIATE indication
				// Do nothing: notification is implicit since this function is only
				// called by receive_pdu
			}
		}

		void StateMachine::AE_7(EventData& data)
		{
			send_pdu(data, 0x02);
		}

		void StateMachine::AE_8(EventData& data)
		{
			send_pdu(data, 0x03);
			start_timer(data);
		}

		void StateMachine::DT_1(EventData& data)
		{
			send_pdu(data, 0x04);
		}

		void StateMachine::DT_2(EventData&)
		{
			// Do nothing: notification is implicit since this function is only called
			// by receive_pdu
		}

		void StateMachine::AR_1(EventData& data)
		{
			send_pdu(data, 0x05);
		}

		void StateMachine::AR_2(EventData&)
		{
			// Do nothing: notification is implicit since this function is only called
			// by receive_pdu
		}

		void StateMachine::AR_3(EventData&)
		{
			// Notification is implicit since this function is only called by
			// receive_pdu
			m_transport.close();
		}

		void StateMachine::AR_4(EventData& data)
		{
			send_pdu(data, 0x06);
			start_timer(data);
		}

		void StateMachine::AR_5(EventData&)
		{
			stop_timer();
		}

		void StateMachine::AR_6(EventData&)
		{
			// Do nothing: notification is implicit since this function is only called
			// by receive_pdu
		}

		void StateMachine::AR_7(EventData& data)
		{
			send_pdu(data, 0x04);
		}

		void StateMachine::AR_8(EventData&)
		{
			// Do nothing: notification is implicit since this function is only called
			// by receive_pdu
		}

		void StateMachine::AR_9(EventData& data)
		{
			send_pdu(data, 0x06);
		}

		void StateMachine::AR_10(EventData&)
		{
			// Do nothing: notification is implicit since this function is only called
			// by receive_pdu
		}

		void StateMachine::AA_1(EventData& data)
		{
			if (std::dynamic_pointer_cast<odil::pdu::AAbort>(data.pdu))
			{
				send_pdu(data, 0x07);
			}
			else
			{
				data.pdu = std::make_shared<odil::pdu::AAbort>(1, 2);
				send_pdu(data);
			}

			start_timer(data);
		}

		void StateMachine::AA_2(EventData&)
		{
			stop_timer();
			m_transport.close();
		}

		void StateMachine::AA_3(EventData&)
		{
			// Notification is implicit since this function is only called
			// by receive_pdu
			m_transport.close();
		}

		void StateMachine::AA_4(EventData&)
		{
			// Do nothing: notification is implicit since this function is only called
			// by receive_pdu
		}

		void StateMachine::AA_5(EventData&)
		{
			stop_timer();
		}

		void StateMachine::AA_6(EventData&)
		{
			// Nothing to do.
		}

		void StateMachine::AA_7(EventData& data)
		{
			send_pdu(data, 0x07);
		}

		void StateMachine::AA_8(EventData& data)
		{
			data.pdu = std::make_shared<odil::pdu::AAbort>(2, 2);
			send_pdu(data, 0x07);
			// Notification is implicit
			start_timer(data);
		}
	}
}
