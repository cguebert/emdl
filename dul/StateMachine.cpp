#include <emdl/dul/StateMachine.h>
#include <emdl/dul/EventData.h>
#include <emdl/association/Association.h>

#include <emdl/pdu/AAbort.h>
#include <emdl/pdu/AAssociateRJ.h>
#include <emdl/pdu/AReleaseRP.h>
#include <emdl/pdu/AReleaseRQ.h>
#include <emdl/pdu/PDataTF.h>

#include <sstream>

namespace emdl
{
	namespace dul
	{
		StateMachine::StateMachine(Association& association, boost::asio::io_service& service)
			: m_association(association)
			, m_transport(std::make_shared<Transport>(service))
			, m_artimTimer(service)
		{
			setState(StateId::Sta1);
		}

		StateMachine::~StateMachine()
		{
			m_transport->setStateMachine(nullptr);
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
			return *m_transport;
		}

		Transport& StateMachine::transport()
		{
			return *m_transport;
		}

		StateMachine::duration_type StateMachine::artimTimeout() const
		{
			return m_timeout;
		}

		void StateMachine::setArtimTimeout(duration_type timeout)
		{
			m_timeout = timeout;
		}

		void StateMachine::setTransportConnection(Transport::Socket socket)
		{
			EventData data;
			transition(Event::TransportConnectionIndication, data);
			m_transport->setStateMachine(shared_from_this());
			m_transport->setSocket(std::move(socket));
		}

		void StateMachine::onTransportClose()
		{
			m_association.setStatus(Association::Status::Closed);
			EventData data;
			transition(Event::TransportConnectionClosedIndication, data);
		}

		void StateMachine::sendPdu(EventData& data)
		{
			if (!data.pdu)
				throw Exception("No PDU");

			switch (data.pdu->type())
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

		void StateMachine::onReceivedPDU(PDUHeader header, std::string body)
		{
			std::stringstream stream;
			stream.write(reinterpret_cast<char*>(&header), sizeof(PDUHeader));
			stream.write(&body[0], body.size());

			EventData data;
			Event event = Event::None;
			switch (header.type)
			{
			case 0x01:
				data.pdu = std::make_shared<pdu::AAssociateRQ>(stream);
				event = Event::AAssociateRQRemote;
				break;
			case 0x02:
				data.pdu = std::make_shared<pdu::AAssociateAC>(stream);
				event = Event::AAssociateACRemote;
				break;
			case 0x03:
				data.pdu = std::make_shared<pdu::AAssociateRJ>(stream);
				event = Event::AAssociateRJRemote;
				break;
			case 0x04:
				data.pdu = std::make_shared<pdu::PDataTF>(stream);
				event = Event::PDataTFRemote;
				break;
			case 0x05:
				data.pdu = std::make_shared<pdu::AReleaseRQ>(stream);
				event = Event::AReleaseRQRemote;
				break;
			case 0x06:
				data.pdu = std::make_shared<pdu::AReleaseRP>(stream);
				event = Event::AReleaseRPRemote;
				break;
			case 0x07:
			{
				auto abortPdu = std::make_shared<pdu::AAbort>(stream);
				m_abortParameters.first = abortPdu->source.get();
				m_abortParameters.second = abortPdu->reason.get();
				data.pdu = abortPdu;
				event = Event::AAbortRemote;
				break;
			}
			default:
				event = Event::InvalidPDU;
			}

			transition(event, data);
		}

		void StateMachine::release()
		{
			auto pdu = std::make_shared<pdu::AReleaseRQ>();
			dul::EventData data;
			data.pdu = pdu;
			sendPdu(data);
		}

		void StateMachine::abort(int source, int reason)
		{
			m_abortParameters.first = source;
			m_abortParameters.second = reason;

			auto pdu = std::make_shared<pdu::AAbort>(source, reason);
			dul::EventData data;
			data.pdu = pdu;
			sendPdu(data);
		}

		void StateMachine::startTimer(EventData& data)
		{
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
			m_artimTimer.cancel();
		}

		const AssociationAcceptor& StateMachine::associationAcceptor() const
		{
			return m_associationAcceptor;
		}

		void StateMachine::setAssociationAcceptor(const AssociationAcceptor& acceptor)
		{
			m_associationAcceptor = acceptor;
		}

		std::pair<unsigned char, unsigned char> StateMachine::abortParameters() const
		{
			return m_abortParameters;
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

		void StateMachine::sendPdu(EventData& data, uint8_t pduType)
		{
			if (!data.pdu)
				throw Exception("No PDU");

			if (data.pdu->type() != pduType)
				throw Exception("Invalid PDU");

			std::ostringstream stream;
			data.pdu->save(stream);
			m_transport->write(stream.str());
		}

		void StateMachine::AE_1(EventData& data)
		{
			if (!data.endpoint)
				throw Exception("Peer endpoint not set");

			m_transport->setStateMachine(shared_from_this());
			m_transport->connect(*data.endpoint);
		}

		void StateMachine::AE_2(EventData& data)
		{
			sendPdu(data, 0x01);
		}

		void StateMachine::AE_3(EventData& data)
		{
			m_association.setStatus(Association::Status::Associated);
			m_association.onAssociationResponse(data);
		}

		void StateMachine::AE_4(EventData& data)
		{
			m_association.setStatus(Association::Status::Rejected);
			m_association.onAssociationResponse(data);
			m_transport->close();
		}

		void StateMachine::AE_5(EventData& data)
		{
			m_association.setStatus(Association::Status::Connected);
			startTimer(data);
		}

		void StateMachine::AE_6(EventData& data)
		{
			stopTimer();

			try
			{
				const AssociationParameters input_parameters{*std::dynamic_pointer_cast<pdu::AAssociateRQ>(data.pdu)};
				data.associationParameters = m_associationAcceptor(input_parameters);
				setState(StateId::Sta3);

				m_association.onAssociationRequest(data); // Issue A-ASSOCIATE indication
			}
			catch (const AssociationRejected& reject)
			{
				data.reject = reject;

				setState(StateId::Sta13);
				startTimer(data);

				data.pdu = std::make_shared<pdu::AAssociateRJ>(reject.result, reject.source, reject.reason);
				sendPdu(data, 0x03);
				data.pdu = nullptr;

				m_association.onAssociationRejected(data);
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

		void StateMachine::DT_2(EventData& data)
		{
			m_association.onPDataTF(data);
		}

		void StateMachine::AR_1(EventData& data)
		{
			sendPdu(data, 0x05);
		}

		void StateMachine::AR_2(EventData&)
		{
			m_association.setStatus(Association::Status::Released);

			// Respond immediately
			EventData data;
			data.pdu = std::make_shared<pdu::AReleaseRP>();
			sendPdu(data);
		}

		void StateMachine::AR_3(EventData&)
		{
			m_association.setStatus(Association::Status::Released);
			m_transport->close();
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

		void StateMachine::AR_6(EventData& data)
		{
			m_association.onPDataTF(data);
		}

		void StateMachine::AR_7(EventData& data)
		{
			sendPdu(data, 0x04);
		}

		void StateMachine::AR_8(EventData&)
		{
			m_association.setStatus(Association::Status::Released);
		}

		void StateMachine::AR_9(EventData& data)
		{
			sendPdu(data, 0x06);
		}

		void StateMachine::AR_10(EventData&)
		{
			m_association.setStatus(Association::Status::Released);
		}

		void StateMachine::AA_1(EventData& data)
		{
			if (std::dynamic_pointer_cast<pdu::AAbort>(data.pdu))
			{
				sendPdu(data, 0x07);
			}
			else
			{
				data.pdu = std::make_shared<pdu::AAbort>(1, 2);
				sendPdu(data);
			}

			startTimer(data);
		}

		void StateMachine::AA_2(EventData&)
		{
			stopTimer();
			m_transport->close();
		}

		void StateMachine::AA_3(EventData&)
		{
			m_association.setStatus(Association::Status::Aborted);
			m_transport->close();
		}

		void StateMachine::AA_4(EventData&)
		{
			m_association.setStatus(Association::Status::Aborted);
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
			data.pdu = std::make_shared<pdu::AAbort>(2, 2);
			sendPdu(data, 0x07);
			m_association.setStatus(Association::Status::Aborted);
			startTimer(data);
		}
	}
}
