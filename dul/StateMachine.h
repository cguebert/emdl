#pragma once

#include <functional>
#include <map>
#include <tuple>
#include <utility>
#include <vector>

#include <boost/asio.hpp>

#include <emdl/dul/EventData.h>
#include <emdl/dul/Transport.h>

#include "odil/AssociationAcceptor.h"

namespace emdl
{
	namespace dul
	{
		/// State machine for the DICOM upper layer.
		class EMDL_API StateMachine
		{
		public:
			/// States of the state machine.
			enum class StateId
			{
				Sta1,
				Sta2,
				Sta3,
				Sta4,
				Sta5,
				Sta6,
				Sta7,
				Sta8,
				Sta9,
				Sta10,
				Sta11,
				Sta12,
				Sta13
			};

			/// Event causing the transitions.
			enum class Event
			{
				None, // dummy event to allow easier initialization
				TransportConnectionIndication,
				TransportConnectionConfirmation,
				TransportConnectionClosedIndication,

				AAssociateRQLocal,
				AAssociateRQRemote,
				AAssociateACLocal,
				AAssociateACRemote,
				AAssociateRJLocal,
				AAssociateRJRemote,

				AReleaseRQLocal,
				AReleaseRQRemote,
				AReleaseRPLocal,
				AReleaseRPRemote,

				PDataTFLocal,
				PDataTFRemote,

				AAbortLocal,
				AAbortRemote,

				ARTIMTimerExpired,
				InvalidPDU,
			};

			/// Duration of the timeout.
			using duration_type = boost::asio::deadline_timer::duration_type;

			StateMachine(); /// Constructor, initializing to Sta1.

			/// Perform the transition related to the event and current state. Raise an exception if no such transition exists.
			void transition(Event event, EventData& data);

			StateId state() const; /// Return the current state.

			const Transport& transport() const; /// Return the TCP transport.
			Transport& transport();             /// Return the TCP transport.

			duration_type timeout() const;          /// Return the timeout, default to infinity.
			void setTimeout(duration_type timeout); /// Set the timeout.

			void setSocket(std::shared_ptr<Transport::Socket> socket); /// Set the socket for the transport and perform the corresponding transition.

			void sendPdu(EventData& data);    /// Send a PDU to the transport, perform the corresponding transition.
			void receivePdu(EventData& data); /// Receive a PDU on the transport, perform the corresponding transition.
			void startTimer(EventData& data); /// Start (or re-start if already started) the ARTIM timer.
			void stopTimer();                 /// Stop the ARTIM timer.

			const odil::AssociationAcceptor& associationAcceptor() const;           /// Return the callback checking whether the association request is acceptable.
			void setAssociationAcceptor(const odil::AssociationAcceptor& acceptor); /// Set the callback checking whether the association request is acceptable.

		private:
			// clang-format off
			enum class Action
			{
				AE_1, AE_2, AE_3, AE_4, AE_5, AE_6, AE_7, AE_8, 
				DT_1, DT_2, 
				AR_1, AR_2, AR_3, AR_4, AR_5, AR_6, AR_7, AR_8, AR_9, AR_10,
				AA_1, AA_2, AA_3, AA_4, AA_5, AA_6, AA_7, AA_8
			};
			// clang-format on

			struct Transition
			{
				Event event;
				Action action;
				StateId nextState;
			};
			using Transitions = std::vector<Transition>;

			struct State
			{
				StateId id;
				Transitions transitions;
			};
			using States = std::vector<State>;

			using ActionFunction = void (StateMachine::*)(EventData&);
			using ActionList = std::vector<ActionFunction>;

			static const States m_states;
			static const ActionList m_actions;

			const State* m_currentState = nullptr;                  /// Current state.
			Transport m_transport;                                  /// TCP transport.
			duration_type m_timeout = boost::posix_time::pos_infin; /// Timeout of the ARTIM timer.
			boost::asio::deadline_timer m_artimTimer;               /// Association Request/Reject/Release Timer.
			odil::AssociationAcceptor m_associationAcceptor;        /// Callback checking whether an association request is acceptable.

			void setState(StateId state); /// Change the current state

			void sendPdu(EventData& data, uint8_t pdu_type); /// Check the PDU type in data and send it.

			// Association establishment
			void AE_1(EventData& data); /// Issue TRANSPORT CONNECT request primitive to local transport service.
			void AE_2(EventData& data); /// Send A-ASSOCIATE-RQ-PDU.
			void AE_3(EventData& data); /// Issue A-ASSOCIATE confirmation (accept) primitive.
			void AE_4(EventData& data); /// Issue A-ASSOCIATE confirmation (reject) primitive and close transport connection.
			void AE_5(EventData& data); /// Issue Transport connection response primitive; start ARTIM timer.
			void AE_6(EventData& data); /// Stop ARTIM timer and accept or reject connection.
			void AE_7(EventData& data); /// Send A-ASSOCIATE-AC PDU.
			void AE_8(EventData& data); /// Send A-ASSOCIATE-RJ PDU and start ARTIM timer.

			// Data transfer
			void DT_1(EventData& data); /// Send P-DATA-TF PDU.
			void DT_2(EventData& data); /// Send P-DATA indication primitive.

			// Association release
			void AR_1(EventData& data);  /// Send A-RELEASE-RQ PDU.
			void AR_2(EventData& data);  /// Issue A-RELEASE indication primitive.
			void AR_3(EventData& data);  /// Issue A-RELEASE confirmation primitive, and close transport connection.
			void AR_4(EventData& data);  /// Issue A-RELEASE-RP PDU and start ARTIM timer.
			void AR_5(EventData& data);  /// Stop ARTIM timer.
			void AR_6(EventData& data);  /// Issue P-DATA indication.
			void AR_7(EventData& data);  /// Issue P-DATA-TF PDU.
			void AR_8(EventData& data);  /// Issue A-RELEASE indication (release collision).
			void AR_9(EventData& data);  /// Send A-RELEASE-RP PDU.
			void AR_10(EventData& data); /// Issue A-RELEASE confirmation primitive.

			// Association abort
			void AA_1(EventData& data); /// Send A-ABORT PDU (service-user source) and start (or restart if already started) ARTIM timer.
			void AA_2(EventData& data); /// Stop ARTIM timer if running. Close transport connection.
			void AA_3(EventData& data); /// Issue A-ABORT (or A-P-ABORT) indication and close transport connection.
			void AA_4(EventData& data); /// Issue A-P-ABORT indication primitive.
			void AA_5(EventData& data); /// Stop ARTIM timer.
			void AA_6(EventData& data); /// Ignore PDU.
			void AA_7(EventData& data); /// Send A-ABORT PDU.
			void AA_8(EventData& data); /// Send A-ABORT PDU (service-provider source-), issue an A-P-ABORT indication, and start ARTIM timer.
		};
	}
}
