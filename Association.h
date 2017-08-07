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
	class EMDL_API Association
	{
	public:
		/// Association result (ITU-T X.227, PS 3.8, 7.1.1.7 and PS 3.8, 9.3.4).
		enum Result
		{
			Accepted = 0,
			RejectedPermanent = 1,
			RejectedTransient = 2,
		};

		/// Source of association result (PS 3.8, 7.1.1.8 and PS 3.8, 9.3.4).
		enum ResultSource
		{
			ULServiceUser = 1,
			ULServiceProviderACSERelatedFunction = 2,
			ULServiceProvderPresentationRelatedFunction = 3,
		};

		/// PS 3.8, 7.1.1.9, and PS 3.8, 9.3.4, and ITU-T X.227
		/// (UL service-user, UL service-provider ACSE-related function),
		/// ITU-T X.226 (UL service-provider presentation-related function)
		/// Diagnostic of association result
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

		/// Duration of the timeout.
		typedef dul::StateMachine::duration_type duration_type;

		Association();                         /// Create a default, un-associated, association.
		Association(const Association& other); /// Create an un-associated association.

		dul::Transport& transport(); /// Return the TCP transport.

		/// Assing an un-associated association; it remains un-associated.
		Association& operator=(const Association& other);

		/// @name Peer
		/// @{
		const std::string& peerHost() const;       /// Return the host name of the peer. Defaults to "".
		void setPeerHost(const std::string& host); /// Set the host name of the peer.
		uint16_t peerPort() const;                 /// Return the port of the peer. Defaults to 104.
		void setPeerPort(uint16_t port);           /// Set the port of the peer.
		/// @}

		/// @name Association parameters
		/// @{
		const odil::AssociationParameters& parameters() const;           /// Return the association parameters.
		odil::AssociationParameters& updateParameters();                 /// Return the association parameters.
		void setParameters(const odil::AssociationParameters& value);    /// Set the association parameters, throw an exception when associated.
		const odil::AssociationParameters& negotiatedParameters() const; /// Return the negotiated association parameters.
		/// @}

		/// @name Timeouts
		/// @{
		duration_type messageTimeout() const;                  /// Return the DIMSE timeout, default to 30s.
		void setMessageTimeout(const duration_type& duration); /// Set the DIMSE timeout.
		/// @}

		/// @name Association
		/// @{
		bool isAssociated() const; /// Test whether the object is currently associated to its peer.
		void associate();          /// Request an association with the peer.

		/// Receive an association from a peer using an opened socket.
		void receiveAssociation(std::shared_ptr<dul::Transport::Socket> open_socket,
								odil::AssociationAcceptor acceptor = odil::default_association_acceptor);

		/// Receive an association from a peer.
		void receiveAssociation(const boost::asio::ip::tcp& protocol, unsigned short port,
								odil::AssociationAcceptor acceptor = odil::default_association_acceptor);

		void release();                     /// Gracefully release the association. Throws an exception if not associated.
		void abort(int source, int reason); /// Forcefully release the association. Throws an exception if not associated.
		/// @}

		/// @name DIMSE messages sending and reception.
		/// @{
		uint16_t nextMessageId();                                    /// Return the next available message id.
		std::string transferSyntaxById(int presentation_context_id); /// Return the transfer syntax corresponding to this context id
		dul::StateMachine& stateMachine();                           /// Return the state machine used by this association

		/// @}

	private:
		dul::StateMachine m_stateMachine;

		std::string m_peerHost;
		uint16_t m_peerPort = 104;

		odil::AssociationParameters m_associationParameters;
		odil::AssociationParameters m_negotiatedParameters;

		std::map<uint8_t, std::string> m_transferSyntaxesById;

		uint16_t m_nextMessageId = 1;
	};

	/// Exception reported when receiving a message after the association was released.
	class EMDL_API AssociationReleased : public Exception
	{
	public:
		AssociationReleased()
			: Exception("Association released")
		{
		}
	};

	/// Exception reported when receiving a message after the association was aborted.
	class EMDL_API AssociationAborted : public Exception
	{
	public:
		AssociationAborted(unsigned char source, unsigned char reason)
			: Exception("Association aborted")
			, source(source)
			, reason(reason)
		{
		}

		uint8_t source; /// Source of the error.
		uint8_t reason; /// Reason of the error.
	};
}