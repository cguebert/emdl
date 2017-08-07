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
		/// @brief State machine for the DICOM upper layer.
		class EMDL_API StateMachine
		{
		public:
			/// @brief States of the state machine.
			enum class State
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

			/// @brief Event causing the transitions.
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

			/// @brief Duration of the timeout.
			typedef boost::asio::deadline_timer::duration_type duration_type;

			/// @brief Constructor, initializing to Sta1.
			StateMachine();

			/**
			 * @brief Perform the transition related to the event and current state.
			 * Raise an exception if no such transition exists.
			 */
			void transition(const Event& event, EventData& data);

			/// @brief Return the current state.
			State get_state() const;

			/// @brief Return the TCP transport.
			const Transport& get_transport() const;

			/// @brief Return the TCP transport.
			Transport& get_transport();

			/// @brief Return the timeout, default to infinity.
			duration_type get_timeout() const;

			/// @brief Set the timeout.
			void set_timeout(duration_type timeout);

			/**
			 * @brief Receive a connection on the TCP transport, perform the
			 * corresponding transition.
			 */
			void receive(EventData& data);

			/// @brief Send a PDU to the transport, perform the corresponding transition.
			void send_pdu(EventData& data);

			/// @brief Receive a PDU on the transport, perform the corresponding transition.
			void receive_pdu(EventData& data);

			/// @brief Start (or re-start if already started) the ARTIM timer.
			void start_timer(EventData& data);

			/// @brief Stop the ARTIM timer.
			void stop_timer();

			/**
			 * @brief Return the callback checking whether the association request is
			 * acceptable.
			 *
			 * By default, all association requests are accepted.
			 */
			const odil::AssociationAcceptor& get_association_acceptor() const;

			/**
			 * @brief Set the callback checking whether the association request is
			 * acceptable.
			 */
			void set_association_acceptor(const odil::AssociationAcceptor& acceptor);

		private:
			enum class Action
			{
				AE_1,
				AE_2,
				AE_3,
				AE_4,
				AE_5,
				AE_6,
				AE_7,
				AE_8,
				DT_1,
				DT_2,
				AR_1,
				AR_2,
				AR_3,
				AR_4,
				AR_5,
				AR_6,
				AR_7,
				AR_8,
				AR_9,
				AR_10,
				AA_1,
				AA_2,
				AA_3,
				AA_4,
				AA_5,
				AA_6,
				AA_7,
				AA_8
			};

			typedef std::map<
				std::tuple<State, Event, bool>,
				std::pair<Action, State>>
				TransitionMap;

			typedef std::map<
				std::pair<State, Event>,
				std::function<bool(const StateMachine&, EventData&)>>
				GuardMap;

			typedef void (StateMachine::*ActionFunction)(EventData&);
			typedef std::vector<ActionFunction> ActionList;

			static const TransitionMap m_transitions;
			static const GuardMap m_guards;
			static const ActionList m_actions;

			/// @brief Current state.
			State m_state;

			/// @brief TCP transport.
			Transport m_transport;

			/// @brief Timeout of the ARTIM timer.
			duration_type m_timeout;

			/// @brief Association Request/Reject/Release Timer.
			boost::asio::deadline_timer m_artim_timer;

			/// @brief Callback checking whether an association request is acceptable.
			odil::AssociationAcceptor m_association_acceptor;

			/// @brief Check the PDU type in data and send it.
			void send_pdu(EventData& data, uint8_t pdu_type);

			/**
			 * @brief Issue TRANSPORT CONNECT request primitive to local transport
			 * service.
			 */
			void AE_1(EventData& data);

			/// @brief Send A-ASSOCIATE-RQ-PDU.
			void AE_2(EventData& data);

			/// @brief Issue A-ASSOCIATE confirmation (accept) primitive.
			void AE_3(EventData& data);

			/**
			 * @brief Issue A-ASSOCIATE confirmation (reject) primitive and close
			 * transport connection.
			 */
			void AE_4(EventData& data);

			/// @brief Issue Transport connection response primitive; start ARTIM timer.
			void AE_5(EventData& data);

			/// @brief Stop ARTIM timer and accept or reject connection.
			void AE_6(EventData& data);

			/// @brief Send A-ASSOCIATE-AC PDU.
			void AE_7(EventData& data);

			/// @brief Send A-ASSOCIATE-RJ PDU and start ARTIM timer.
			void AE_8(EventData& data);

			/// @brief Send P-DATA-TF PDU.
			void DT_1(EventData& data);

			/// @brief Send P-DATA indication primitive.
			void DT_2(EventData& data);

			/// @brief Send A-RELEASE-RQ PDU.
			void AR_1(EventData& data);

			/// @brief Issue A-RELEASE indication primitive.
			void AR_2(EventData& data);

			/**
			 * @brief Issue A-RELEASE confirmation primitive, and close transport.
			 * connection.
			 */
			void AR_3(EventData& data);

			/// @brief Issue A-RELEASE-RP PDU and start ARTIM timer.
			void AR_4(EventData& data);

			/// @brief Stop ARTIM timer.
			void AR_5(EventData& data);

			/// @brief Issue P-DATA indication.
			void AR_6(EventData& data);

			/// @brief Issue P-DATA-TF PDU.
			void AR_7(EventData& data);

			/// @brief Issue A-RELEASE indication (release collision).
			void AR_8(EventData& data);

			/// @brief Send A-RELEASE-RP PDU.
			void AR_9(EventData& data);

			/// @brief Issue A-RELEASE confirmation primitive.
			void AR_10(EventData& data);

			/**
			 * @brief Send A-ABORT PDU (service-user source) and start (or restart if
			 * already started) ARTIM timer.
			 */
			void AA_1(EventData& data);

			/// @brief Stop ARTIM timer if running. Close transport connection.
			void AA_2(EventData& data);

			/**
			 * @brief If (service-user inititated abort): issue A-ABORT indication and
			 * close transport connection ; otherwise (service-provider inititated
			 * abort): issue A-P-ABORT indication and close transport connection.
			 */
			void AA_3(EventData& data);

			/// @brief Issue A-P-ABORT indication primitive.
			void AA_4(EventData& data);

			/// @brief Stop ARTIM timer.
			void AA_5(EventData& data);

			/// @brief Ignore PDU.
			void AA_6(EventData& data);

			/// @brief Send A-ABORT PDU.
			void AA_7(EventData& data);

			/**
			 * @brief Send A-ABORT PDU (service-provider source-), issue an A-P-ABORT
			 * indication, and start ARTIM timer.
			 */
			void AA_8(EventData& data);
		};
	}
}
