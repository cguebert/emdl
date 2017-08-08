#include <emdl/Association.h>
#include <emdl/dul/EventData.h>

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
	Association::Association()
		: m_stateMachine(*this)
	{
		setMessageTimeout(boost::posix_time::seconds(30));
	}

	Association::Association(const Association& other)
		: m_stateMachine(*this)
		, m_peerHost(other.m_peerHost)
		, m_peerPort(other.m_peerPort)
		, m_associationParameters(other.m_associationParameters)
		, m_nextMessageId(other.m_nextMessageId)
	{
		setMessageTimeout(other.messageTimeout());
	}

	dul::Transport& Association::transport()
	{
		return m_stateMachine.transport();
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

	Association::duration_type Association::messageTimeout() const
	{
		return m_stateMachine.timeout();
	}

	void Association::setMessageTimeout(const duration_type& duration)
	{
		m_stateMachine.setTimeout(duration);
	}

	bool Association::isAssociated() const
	{
		return m_stateMachine.transport().isOpen() && m_stateMachine.state() == dul::StateMachine::StateId::Sta6;
	}

	void Association::associate()
	{
		boost::asio::ip::tcp::resolver resolver(m_stateMachine.transport().service());
		const boost::asio::ip::tcp::resolver::query query(m_peerHost, "");
		const auto endpoint_it = resolver.resolve(query);

		dul::EventData data;
		data.endpoint = *endpoint_it;
		data.endpoint->port(m_peerPort);

		const auto request = std::make_shared<odil::pdu::AAssociateRQ>(m_associationParameters.as_a_associate_rq());

		data.pdu = request;

		m_stateMachine.sendPdu(data);
		m_stateMachine.receivePdu(data);

		if (!data.pdu)
		{
			throw Exception("No response received");
		}
		else
		{
			const auto acceptation = std::dynamic_pointer_cast<odil::pdu::AAssociateAC>(data.pdu);
			const auto rejection = std::dynamic_pointer_cast<odil::pdu::AAssociateRJ>(data.pdu);
			if (acceptation)
			{
				m_negotiatedParameters = odil::AssociationParameters{*acceptation, m_associationParameters};

				m_transferSyntaxesById.clear();

				for (const auto& pc : m_negotiatedParameters.get_presentation_contexts())
				{
					if (pc.result == odil::AssociationParameters::PresentationContext::Result::Acceptance)
					{
						m_transferSyntaxesById[pc.id] = pc.transfer_syntaxes[0];
					}
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

	void Association::receiveAssociation(std::shared_ptr<dul::Transport::Socket> socket, odil::AssociationAcceptor acceptor)
	{
		m_stateMachine.setAssociationAcceptor(acceptor);

		m_stateMachine.setSocket(std::move(socket));

		dul::EventData data;
		m_stateMachine.receivePdu(data);

		if (!data.pdu)
		{
			// We have rejected the request
			if (data.reject)
			{
				throw(*data.reject);
			}
			else
			{
				throw odil::AssociationRejected(Association::RejectedTransient,
												Association::ULServiceProvderPresentationRelatedFunction,
												Association::NoReasonGiven,
												"No reject information");
			}
		}
		else
		{
			const auto& request = std::dynamic_pointer_cast<odil::pdu::AAssociateRQ>(data.pdu);
			if (!request)
				throw Exception("Invalid response");

			const auto endpoint = m_stateMachine.transport().socket()->remote_endpoint();
			m_peerHost = endpoint.address().to_string();
			m_peerPort = endpoint.port();

			if (!data.associationParameters)
				throw Exception("Association parameters not set");
			m_negotiatedParameters = *data.associationParameters;

			m_transferSyntaxesById.clear();

			for (const auto& pc : m_negotiatedParameters.get_presentation_contexts())
			{
				if (pc.result == odil::AssociationParameters::PresentationContext::Result::Acceptance)
					m_transferSyntaxesById[pc.id] = pc.transfer_syntaxes[0];
			}

			data.pdu = std::make_shared<odil::pdu::AAssociateAC>(m_negotiatedParameters.as_a_associate_ac());
			m_stateMachine.sendPdu(data);
		}
	}

	void Association::receiveAssociation(const boost::asio::ip::tcp& protocol, unsigned short port, odil::AssociationAcceptor acceptorFunc)
	{
		auto& service = transport().service();
		auto socket = std::make_shared<dul::Transport::Socket>(service);

		dul::Transport::Socket::endpoint_type endpoint{protocol, port};
		boost::asio::ip::tcp::acceptor acceptor{service, endpoint};
		boost::system::error_code ec;
		acceptor.accept(*socket, ec);

		receiveAssociation(socket, acceptorFunc);
	}

	void Association::release()
	{
		if (!isAssociated())
			throw Exception("Not associated");

		auto pdu = std::make_shared<odil::pdu::AReleaseRQ>();
		dul::EventData data;
		data.pdu = pdu;
		m_stateMachine.sendPdu(data);
		m_stateMachine.receivePdu(data);

		const auto reply = std::dynamic_pointer_cast<odil::pdu::AReleaseRP>(data.pdu);
		if (!reply)
		{
			// Invalid response, accept it nevertheless.
		}
	}

	void Association::abort(int source, int reason)
	{
		if (!isAssociated())
			throw Exception("Not associated");

		auto pdu = std::make_shared<odil::pdu::AAbort>(source, reason);
		dul::EventData data;
		data.pdu = pdu;
		m_stateMachine.sendPdu(data);
	}

	uint16_t Association::nextMessageId()
	{
		return ++m_nextMessageId;
	}

	std::string Association::transferSyntaxById(int presentation_context_id)
	{
		const auto transfer_syntax_it = m_transferSyntaxesById.find(presentation_context_id);
		if (transfer_syntax_it == m_transferSyntaxesById.end())
			throw Exception("No such Presentation Context ID");

		return transfer_syntax_it->second;
	}

	dul::StateMachine& Association::stateMachine()
	{
		return m_stateMachine;
	}
}
