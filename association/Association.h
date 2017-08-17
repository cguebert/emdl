#pragma once

#include <atomic>
#include <deque>
#include <functional>
#include <future>
#include <map>
#include <mutex>
#include <string>
#include <vector>

#include <emdl/TransferSyntaxes.h>
#include <emdl/association/AssociationAcceptor.h>
#include <emdl/dataset/DataSet.h>
#include <emdl/dul/StateMachine.h>
#include <emdl/message/Message.h>

namespace emdl
{
	class MessageWrapper
	{
	public:
		MessageWrapper()
		{
			canceled = false;
		}

		std::atomic_bool canceled; /// Can be set to true later for Store, Get and Move operations
		std::shared_ptr<emdl::message::Message> message; /// The message itself

		std::chrono::high_resolution_clock::time_point receptionStart, receptionEnd, processingEnd; // Mostly for debug
	};
	using MessageWrapperSPtr = std::shared_ptr<MessageWrapper>;

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

		enum class Status
		{
			Idle,
			Connected,
			Associated, // Can send data
			Rejected,
			Released,
			Aborted,
			Closed
		};

		/// Duration of the timeout.
		using duration_type = dul::StateMachine::duration_type;

		Association(boost::asio::io_service& service); /// Create a default, un-associated association.
		~Association(); /// Ensure the connection is closed

		dul::Transport& transport(); /// Return the TCP transport.

		/// Assing an un-associated association; it remains un-associated.
		Association& operator=(const Association& other);

		/// @name Peer
		/// @{
		const std::string& peerHost() const; /// Return the host name of the peer. Defaults to "".
		void setPeerHost(const std::string& host); /// Set the host name of the peer.
		uint16_t peerPort() const; /// Return the port of the peer. Defaults to 104.
		void setPeerPort(uint16_t port); /// Set the port of the peer.
		/// @}

		/// @name Association parameters
		/// @{
		const AssociationParameters& parameters() const; /// Return the association parameters.
		AssociationParameters& updateParameters(); /// Return the association parameters.
		void setParameters(const AssociationParameters& value); /// Set the association parameters, throw an exception when associated.
		const AssociationParameters& negotiatedParameters() const; /// Return the negotiated association parameters.
		/// @}

		/// @name Association
		/// @{
		bool isAssociated() const; /// Test whether the object is currently associated to its peer.
		void associate(); /// Request an association with the peer.

		/// Receive an association from a peer using an opened socket.
		void receiveAssociation(dul::Transport::Socket open_socket,
								AssociationAcceptor acceptor = defaultAssociationAcceptor);

		/// Receive an association from a peer.
		void receiveAssociation(const boost::asio::ip::tcp& protocol, unsigned short port,
								AssociationAcceptor acceptor = defaultAssociationAcceptor);
		void waitForReleaseResponse(); /// Blocks until a release response has been received
		/// @}

		/// @name DIMSE messages sending and reception.
		/// @{
		uint16_t nextMessageId(); /// Return the next available message id.
		TransferSyntax transferSyntaxById(int presentation_context_id); /// Return the transfer syntax corresponding to this context id
		dul::StateMachine& stateMachine(); /// Return the state machine used by this association

		/// @}

		MessageWrapperSPtr popMessage(); /// Block and return the next message. Raise an exception if the association has been released, aborted or closed.
		void sendMessage(const message::Message& message, uint8_t presentationContextId, TransferSyntax transferSyntax);

		/// @name Called by the state machine
		/// @{
		void setStatus(Status status);

		void onAssociationRequest(dul::EventData& data);
		void onAssociationRejected(dul::EventData& data);
		void onAssociationResponse(dul::EventData& data);
		void onPDataTF(dul::EventData& data);
		/// @}

	private:
		void supportCancellation(const MessageWrapperSPtr& wrapper);

		uint16_t m_nextMessageId = 1;
		std::shared_ptr<dul::StateMachine> m_stateMachine;

		std::string m_peerHost;
		uint16_t m_peerPort = 104;
		Status m_status;

		std::map<uint8_t, TransferSyntax> m_transferSyntaxesById;

		AssociationParameters m_associationParameters, m_negotiatedParameters;

		std::promise<void> m_associationRequestPromise, m_associationReleasedPromise;
		std::promise<dul::EventData> m_associationResponsePromise;

		struct MessageConstruction
		{
			int presentationContextId = 0;
			bool started = false; // Set to true when we have received something
			bool receivedCommandSet = false;
			bool receivedDataSet = false;
			bool hasDataSet = true;
			DataSet commandSet;
			std::stringstream commandStream, dataStream;
			std::chrono::high_resolution_clock::time_point receptionStart;
		};
		MessageConstruction m_readMessage;

		std::deque<MessageWrapperSPtr> m_messagesQueue;
		std::mutex m_messagesQueueMutex;
		std::condition_variable m_messagesCondition;

		using WrapperPair = std::pair<Value::Integer, std::weak_ptr<MessageWrapper>>;
		std::vector<WrapperPair> m_cancelableWrappers;
		std::mutex m_cancelableWrappersMutex;
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
