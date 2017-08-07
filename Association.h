#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <string>
#include <vector>

#include <emdl/dul/StateMachine.h>

#include "odil/AssociationAcceptor.h"
#include "odil/AssociationParameters.h"
#include "odil/message/Message.h"

namespace emdl
{
	/**
	 * @brief Association.
	 */
	class EMDL_API Association
	{
	public:
		/// @brief Association result (ITU-T X.227, PS 3.8, 7.1.1.7 and PS 3.8, 9.3.4).
		enum Result
		{
			Accepted = 0,
			RejectedPermanent = 1,
			RejectedTransient = 2,
		};

		/// @brief Source of association result (PS 3.8, 7.1.1.8 and PS 3.8, 9.3.4).
		enum ResultSource
		{
			ULServiceUser = 1,
			ULServiceProviderACSERelatedFunction = 2,
			ULServiceProvderPresentationRelatedFunction = 3,
		};

		// PS 3.8, 7.1.1.9, and PS 3.8, 9.3.4, and ITU-T X.227 (UL service-user,
		// UL service-provider ACSE-related function), ITU-T X.226 (UL
		// service-provider presentation-related function)
		/// @brief Diagnostic of association result
		enum Diagnostic
		{
			// UL service-user
			NoReasonGiven = 1,
			ApplicationContextNameNotSupported = 2,
			CallingAETitleNotRecognized = 3,
			CallingAPInvocationIdentifierNotRecognized = 4,
			CallingAEQualifierNotRecognized = 5,
			CallingAEInvocationIdentifierNotRecognized = 6,
			CalledAETitleNotRecognized = 7,
			CalledAPInvocationIdentifierNotRecognized = 8,
			CalledAEQualifierNotRecognized = 9,
			CalledAEInvocationIdentifierNotRecognized = 10,

			// UL service-provider, ACSE-related function
			NoCommonULVersion = 2,

			// UL service-provider, presentation-related function
			TemporaryCongestion = 1,
			LocalLimitExceeded = 2,
			CalledPresentationAddressUnknown = 3,
			PresentationProtocolVersionNotSupported = 4,
			NoPresentationServiceAccessPointAvailable = 7,
		};

		/// @brief Duration of the timeout.
		typedef dul::StateMachine::duration_type duration_type;

		/// @brief Create a default, un-associated, association.
		Association();

		/// @brief Create an un-associated association.
		Association(const Association& other);

		/// @brief Return the TCP transport.
		dul::Transport& get_transport();

		/// @brief Assing an un-associated association; it remains un-associated.
		Association& operator=(const Association& other);

		/// @name Peer
		/// @{

		/// @brief Return the host name of the peer. Defaults to "".
		const std::string& get_peer_host() const;
		/// @brief Set the host name of the peer.
		void set_peer_host(const std::string& host);

		/// @brief Return the port of the peer. Defaults to 104.
		uint16_t get_peer_port() const;
		/// @brief Set the port of the peer.
		void set_peer_port(uint16_t port);

		/// @}

		/// @brief Return the association parameters.
		const odil::AssociationParameters& get_parameters() const;

		/// @brief Return the association parameters.
		odil::AssociationParameters& update_parameters();

		/// @brief Set the association parameters, throw an exception when associated.
		void set_parameters(const odil::AssociationParameters& value);

		/// @brief Return the negotiated association parameters.
		const odil::AssociationParameters& get_negotiated_parameters() const;

		/// @name Timeouts
		/// @{

		/// @brief Return the TCP timeout, default to infinity.
		duration_type get_tcp_timeout() const;

		/// @brief Set the timeout.
		void set_tcp_timeout(const duration_type& duration);

		/// @brief Return the DIMSE timeout, default to 30s.
		duration_type get_message_timeout() const;

		/// @brief Set the DIMSE timeout.
		void set_message_timeout(const duration_type& duration);

		/// @}

		/// @name Association
		/// @{

		/// @brief Test whether the object is currently associated to its peer.
		bool is_associated() const;

		/// @brief Request an association with the peer.
		void associate();

		/// @brief Receive an association from a peer using an opened socket.
		void receive_association(std::shared_ptr<dul::Transport::Socket> open_socket,
								 odil::AssociationAcceptor acceptor = odil::default_association_acceptor);

		/// @brief Receive an association from a peer.
		void receive_association(const boost::asio::ip::tcp& protocol, unsigned short port,
								 odil::AssociationAcceptor acceptor = odil::default_association_acceptor);

		/// @brief Reject the received association request.
		void reject(Result result, ResultSource result_source, Diagnostic diagnostic);

		/// @brief Gracefully release the association. Throws an exception if not associated.
		void release();
		/// @brief Forcefully release the association. Throws an exception if not associated.
		void abort(int source, int reason);

		/// @}

		/// @name DIMSE messages sending and reception.
		/// @{

		/// @brief Return the next available message id.
		uint16_t next_message_id();

		/// @brief Return the transfer syntax corresponding to this context id
		std::string get_transfer_syntax_by_id(int presentation_context_id);

		/// @brief Return the state machine used by this association
		dul::StateMachine& stateMachine();

		/// @}

	private:
		dul::StateMachine m_stateMachine;

		std::string m_peerHost;
		uint16_t m_peerPort = 104;

		odil::AssociationParameters m_associationParameters;
		odil::AssociationParameters m_negotiatedParameters;

		std::map<uint8_t, std::string> _transfer_syntaxes_by_id;

		uint16_t m_nextMessageId = 1;
	};

	/// @brief Exception reported when receiving a message after the association was released.
	class EMDL_API AssociationReleased : public Exception
	{
	public:
		AssociationReleased()
			: Exception("Association released")
		{
		}
	};

	/// @brief Exception reported when receiving a message after the association was aborted.
	class EMDL_API AssociationAborted : public Exception
	{
	public:
		/// @brief Source of the error.
		uint8_t source;

		/// @brief Reason of the error.
		uint8_t reason;

		/// @brief Constructor.
		AssociationAborted(unsigned char source, unsigned char reason)
			: Exception("Association aborted")
			, source(source)
			, reason(reason)
		{
		}
	};
}
