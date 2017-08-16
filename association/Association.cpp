#include <emdl/association/Association.h>
#include <emdl/dul/EventData.h>
#include <emdl/dataset/DataSetAccessors.h>
#include <emdl/dataset/DataSetReader.h>
#include <emdl/dataset/DataSetWriter.h>

#include <algorithm>
#include <functional>
#include <map>
#include <string>
#include <vector>

#include "odil/AssociationParameters.h"
#include "odil/DataSet.h"
#include "odil/Exception.h"
#include "odil/uid.h"
#include "odil/dul/StateMachine.h"
#include "odil/message/Message.h"
#include "odil/pdu/AAbort.h"
#include "odil/pdu/AAssociate.h"
#include "odil/pdu/AAssociateRJ.h"
#include "odil/pdu/AReleaseRP.h"
#include "odil/pdu/AReleaseRQ.h"
#include "odil/pdu/ImplementationClassUID.h"
#include "odil/pdu/ImplementationVersionName.h"
#include "odil/pdu/PDataTF.h"
#include "odil/pdu/PresentationContextAC.h"
#include "odil/pdu/PresentationContextRQ.h"
#include "odil/pdu/RoleSelection.h"
#include "odil/pdu/UserIdentityRQ.h"
#include "odil/pdu/UserInformation.h"
#include "odil/Reader.h"
#include "odil/Writer.h"

namespace emdl
{
	Association::Association(boost::asio::io_service& service)
		: m_stateMachine(std::make_shared<dul::StateMachine>(*this, service))
	{
	}

	Association::~Association()
	{
		if (m_status == Status::Connected)
			m_stateMachine->transport().close();
		else if (m_status == Status::Associated)
		{
			m_stateMachine->abort(0, 0);
			m_stateMachine->transport().close();
		}
	}

	dul::Transport& Association::transport()
	{
		return m_stateMachine->transport();
	}

	Association& Association::operator=(const Association& other)
	{
		if (this != &other)
		{
			setPeerHost(other.peerHost());
			setPeerPort(other.peerPort());
			setParameters(other.parameters());
		}

		return *this;
	}

	const std::string& Association::peerHost() const
	{
		return m_peerHost;
	}

	void Association::setPeerHost(const std::string& host)
	{
		if (isAssociated())
			throw Exception("Cannot set member while associated");

		m_peerHost = host;
	}

	uint16_t Association::peerPort() const
	{
		return m_peerPort;
	}

	void Association::setPeerPort(uint16_t port)
	{
		if (isAssociated())
			throw Exception("Cannot set member while associated");

		m_peerPort = port;
	}

	const odil::AssociationParameters& Association::parameters() const
	{
		return m_associationParameters;
	}

	odil::AssociationParameters& Association::updateParameters()
	{
		if (isAssociated())
			throw Exception("Cannot set member while associated");

		return m_associationParameters;
	}

	void Association::setParameters(const odil::AssociationParameters& value)
	{
		if (isAssociated())
			throw Exception("Cannot set member while associated");

		m_associationParameters = value;
	}

	const odil::AssociationParameters& Association::negotiatedParameters() const
	{
		return m_negotiatedParameters;
	}

	bool Association::isAssociated() const
	{
		return m_stateMachine->transport().isOpen() && m_stateMachine->state() == dul::StateMachine::StateId::Sta6;
	}

	void Association::associate()
	{
		boost::asio::ip::tcp::resolver resolver(m_stateMachine->transport().service());
		const boost::asio::ip::tcp::resolver::query query(m_peerHost, "");
		const auto endpoint_it = resolver.resolve(query);

		dul::EventData data;
		data.endpoint = *endpoint_it;
		data.endpoint->port(m_peerPort);

		const auto request = std::make_shared<odil::pdu::AAssociateRQ>(m_associationParameters.as_a_associate_rq());
		data.pdu = request;

		m_stateMachine->sendPdu(data);

		auto responseFuture = m_associationResponsePromise.get_future();
		const auto response = responseFuture.get();

		if (!response.pdu)
		{
			throw Exception("No response received");
		}
		else
		{
			const auto acceptation = std::dynamic_pointer_cast<odil::pdu::AAssociateAC>(response.pdu);
			const auto rejection = std::dynamic_pointer_cast<odil::pdu::AAssociateRJ>(response.pdu);
			if (acceptation)
			{
				m_negotiatedParameters = odil::AssociationParameters{*acceptation, m_associationParameters};

				m_transferSyntaxesById.clear();

				for (const auto& pc : m_negotiatedParameters.get_presentation_contexts())
				{
					if (pc.result == odil::AssociationParameters::PresentationContext::Result::Acceptance)
						m_transferSyntaxesById[pc.id] = getTransferSyntax(pc.transfer_syntaxes[0]);
				}
			}
			else if (rejection)
			{
				throw odil::AssociationRejected(rejection->get_result(),
												rejection->get_source(),
												rejection->get_reason(),
												"Association rejected");
			}
			else
			{
				throw Exception("Invalid response");
			}
		}
	}

	void Association::receiveAssociation(dul::Transport::Socket socket, odil::AssociationAcceptor acceptor)
	{
		m_stateMachine->setAssociationAcceptor(acceptor);
		m_stateMachine->setTransportConnection(std::move(socket));

		auto associationRequestFuture = m_associationRequestPromise.get_future();
		associationRequestFuture.get(); // Waits for association, rethrows if there was an exception
	}

	void Association::receiveAssociation(const boost::asio::ip::tcp& protocol, unsigned short port, odil::AssociationAcceptor acceptorFunc)
	{
		auto& service = transport().service();
		auto socket = dul::Transport::Socket{service};

		dul::Transport::Socket::endpoint_type endpoint{protocol, port};
		boost::asio::ip::tcp::acceptor acceptor{service, endpoint};
		boost::system::error_code ec;
		acceptor.accept(socket, ec);

		receiveAssociation(std::move(socket), acceptorFunc);
	}

	void Association::waitForReleaseResponse()
	{
		auto releaseFuture = m_associationReleasedPromise.get_future();
		releaseFuture.get();
	}

	uint16_t Association::nextMessageId()
	{
		return ++m_nextMessageId;
	}

	TransferSyntax Association::transferSyntaxById(int presentation_context_id)
	{
		const auto it = m_transferSyntaxesById.find(presentation_context_id);
		if (it == m_transferSyntaxesById.end())
			throw Exception("No such Presentation Context ID");

		return it->second;
	}

	dul::StateMachine& Association::stateMachine()
	{
		return *m_stateMachine;
	}

	void Association::setStatus(Status status)
	{
		m_status = status;

		switch (status)
		{
		case Status::Released:
			m_associationReleasedPromise.set_value();
			m_messagesCondition.notify_one();
			break;
		case Status::Aborted:
			m_messagesCondition.notify_one();
			break;
		case Status::Closed:
			m_messagesCondition.notify_one();
			break;
		}
	}

	void Association::onAssociationRequest(dul::EventData& data)
	{
		try
		{
			const auto& request = std::dynamic_pointer_cast<odil::pdu::AAssociateRQ>(data.pdu);
			if (!request)
				throw Exception("Invalid response");

			const auto endpoint = m_stateMachine->transport().remoteEndpoint();
			m_peerHost = endpoint.address().to_string();
			m_peerPort = endpoint.port();

			if (!data.associationParameters)
				throw Exception("Association parameters not set");
			m_negotiatedParameters = *data.associationParameters;

			m_transferSyntaxesById.clear();

			for (const auto& pc : m_negotiatedParameters.get_presentation_contexts())
			{
				if (pc.result == odil::AssociationParameters::PresentationContext::Result::Acceptance)
					m_transferSyntaxesById[pc.id] = getTransferSyntax(pc.transfer_syntaxes[0]);
			}

			setStatus(Association::Status::Associated);

			data.pdu = std::make_shared<odil::pdu::AAssociateAC>(m_negotiatedParameters.as_a_associate_ac());
			m_stateMachine->sendPdu(data);

			m_associationRequestPromise.set_value();
		}
		catch (...)
		{
			m_associationRequestPromise.set_exception(std::current_exception());
		}
	}

	void Association::onAssociationRejected(dul::EventData& data)
	{
		setStatus(Association::Status::Rejected);

		// We have rejected the request
		if (data.reject)
			m_associationRequestPromise.set_exception(std::make_exception_ptr(*data.reject));
		else
			m_associationRequestPromise.set_exception(std::make_exception_ptr(
				odil::AssociationRejected(Association::RejectedTransient,
										  Association::ULServiceProvderPresentationRelatedFunction,
										  Association::NoReasonGiven,
										  "No reject information")));
	}

	void Association::onAssociationResponse(dul::EventData& data)
	{
		m_associationResponsePromise.set_value(data);
	}

	void Association::onPDataTF(dul::EventData& data)
	{
		sizeof(MessageConstruction);
		const auto pData = std::dynamic_pointer_cast<odil::pdu::PDataTF>(data.pdu);
		if (!pData)
			throw Exception("Invalid PDU received");

		if (!m_readMessage.started)
		{
			m_readMessage.started = true;
			m_readMessage.receptionStart = std::chrono::high_resolution_clock::now();
		}

		for (const auto& pdv : pData->get_pdv_items())
		{
			m_readMessage.presentationContextId = pdv.get_presentation_context_id();
			bool& received = pdv.is_command() ? m_readMessage.receivedCommandSet : m_readMessage.receivedDataSet;
			received |= pdv.is_last_fragment();

			std::stringstream& stream = pdv.is_command() ? m_readMessage.commandStream : m_readMessage.dataStream;
			const auto& fragment_data = pdv.get_fragment();
			stream.write(&fragment_data[0], fragment_data.size());

			if (m_readMessage.receivedCommandSet && m_readMessage.commandSet.empty())
			{
				auto buffer = createBufferFromStream(m_readMessage.commandStream);
				m_readMessage.commandSet = DataSetReader{buffer, TransferSyntax::ImplicitVRLittleEndian}.readDataSet();
				const auto value = asInt(m_readMessage.commandSet, odil::registry::CommandDataSetType, 0);

				if (value == message::Message::DataSetType::ABSENT)
					m_readMessage.hasDataSet = false;
			}
		}

		if (m_readMessage.receivedCommandSet && (!m_readMessage.hasDataSet || m_readMessage.receivedDataSet))
		{
			auto wrapper = std::make_shared<MessageWrapper>();
			if (m_readMessage.hasDataSet)
			{
				const auto transferSyntax = transferSyntaxById(m_readMessage.presentationContextId);
				auto buffer = createBufferFromStream(m_readMessage.dataStream);
				wrapper->message = std::make_shared<message::Message>(std::move(m_readMessage.commandSet),
																	  std::move(buffer),
																	  transferSyntax);
			}
			else
				wrapper->message = std::make_shared<message::Message>(std::move(m_readMessage.commandSet));

			wrapper->receptionStart = m_readMessage.receptionStart;
			wrapper->receptionEnd = wrapper->processingEnd = std::chrono::high_resolution_clock::now();

			{
				std::lock_guard<std::mutex> lock(m_messagesQueueMutex);
				m_messagesQueue.push_back(wrapper);
			}

			m_messagesCondition.notify_one();

			supportCancellation(wrapper);
			m_readMessage = {}; // Reset
		}
	}

	MessageWrapperSPtr Association::popMessage()
	{
		std::unique_lock<std::mutex> lock(m_messagesQueueMutex);
		if (m_status == Status::Associated && m_messagesQueue.empty()) // Wait until there is a message
		{
			m_messagesCondition.wait(lock, [this]() {
				return !m_messagesQueue.empty() || m_status != Status::Associated;
			});
		}

		if (!m_messagesQueue.empty())
		{
			auto msg = m_messagesQueue.front();
			m_messagesQueue.pop_front();
			return msg;
		}
		else
		{
			switch (m_status)
			{
			case Status::Released:
				throw emdl::AssociationReleased();
			case Status::Aborted:
			{
				const auto params = m_stateMachine->abortParameters();
				throw emdl::AssociationAborted(params.first, params.second);
			}
			case Status::Closed:
				throw dul::SocketClosed("Connection closed");
			default:
				throw Exception("Association not yet ready");
			}
		}
	}

	void Association::sendMessage(const message::Message& message, uint8_t presentationContextId, TransferSyntax transferSyntax)
	{
		std::vector<odil::pdu::PDataTF::PresentationDataValueItem> pdv_items;

		std::ostringstream commandStream;
		DataSetWriter{commandStream, TransferSyntax::ImplicitVRLittleEndian, BaseWriter::ItemEncoding::ExplicitLength}.writeDataSet(message.commandSet());
		const auto commandBuffer = commandStream.str();
		const auto currentLength = commandBuffer.size() + 12; // 12 is the size of all that is added on top of the fragment
		pdv_items.emplace_back(presentationContextId, 3, std::move(commandBuffer));

		if (message.hasDataSet())
		{
			std::ostringstream dataStream;
			DataSetWriter{dataStream, transferSyntax, BaseWriter::ItemEncoding::ExplicitLength}.writeDataSet(message.dataSet());
			const auto dataBuffer = dataStream.str();

			const auto maxLength = negotiatedParameters().get_maximum_length();
			if (!maxLength || (currentLength + dataBuffer.size() + 6 < maxLength))
			{ // Can send all the buffer in one go
				pdv_items.emplace_back(presentationContextId, 2, std::move(dataBuffer));

				emdl::dul::EventData data;
				data.pdu = std::make_shared<odil::pdu::PDataTF>(pdv_items);
				m_stateMachine->sendPdu(data);
			}
			else // We have to fragment into multiple PDUs
			{
				auto available = maxLength - 6 - currentLength; // Need at least 6 bytes for the headers
				int64_t remaining = dataBuffer.size();
				std::size_t offset = 0;

				if (available > 0) // Send some data with the command set
				{
					remaining -= available;
					pdv_items.emplace_back(presentationContextId, (remaining > 0 ? 0 : 2), dataBuffer.substr(0, available));
					offset += available;
				}

				auto pdu = std::make_shared<odil::pdu::PDataTF>(pdv_items);
				emdl::dul::EventData data;
				data.pdu = pdu;
				m_stateMachine->sendPdu(data);

				available = maxLength - 6; // In case some software do not take into account the size of the header when allocating their buffer
				while (remaining > 0)
				{
					remaining -= available;
					pdv_items.clear();
					pdv_items.emplace_back(presentationContextId, (remaining > 0 ? 0 : 2), dataBuffer.substr(offset, available));
					offset += available;
					pdu->set_pdv_items(pdv_items);
					m_stateMachine->sendPdu(data);
				}
			}
		}
		else
		{
			emdl::dul::EventData data;
			data.pdu = std::make_shared<odil::pdu::PDataTF>(pdv_items);
			m_stateMachine->sendPdu(data);
		}
	}

	void Association::supportCancellation(const MessageWrapperSPtr& wrapper)
	{
		using Cmd = message::Message::Command;
		const auto commandField = wrapper->message->commandField.get();
		if (commandField == Cmd::C_FIND_RQ || commandField == Cmd::C_GET_RQ || commandField == Cmd::C_MOVE_RQ)
		{
			std::lock_guard<std::mutex> lock(m_cancelableWrappersMutex);

			// First remove old messages
			const auto last = std::remove_if(m_cancelableWrappers.begin(), m_cancelableWrappers.end(), [](const WrapperPair& p) {
				return p.second.expired();
			});
			m_cancelableWrappers.erase(last, m_cancelableWrappers.end());

			// Add this message
			const auto msgId = *firstInt(wrapper->message->commandSet(), odil::registry::MessageID);
			m_cancelableWrappers.emplace_back(msgId, wrapper);
		}
		else if (commandField == Cmd::C_CANCEL_RQ)
		{
			const auto msgId = *firstInt(wrapper->message->commandSet(), odil::registry::MessageIDBeingRespondedTo);

			std::lock_guard<std::mutex> lock(m_cancelableWrappersMutex);
			const auto it = std::find_if(m_cancelableWrappers.begin(), m_cancelableWrappers.end(), [msgId](const WrapperPair& p) {
				return p.first == msgId;
			});
			if (it != m_cancelableWrappers.end())
				it->second.lock()->canceled = true;
		}
	}
}
