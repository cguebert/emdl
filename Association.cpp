#include <emdl/Association.h>

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
	{
		set_tcp_timeout(boost::posix_time::pos_infin);
		set_message_timeout(boost::posix_time::seconds(30));
	}

	Association::Association(const Association& other)
		: m_peerHost(other.m_peerHost)
		, m_peerPort(other.m_peerPort)
		, m_associationParameters(other.m_associationParameters)
		, m_nextMessageId(other.m_nextMessageId)
	{
		set_tcp_timeout(other.get_tcp_timeout());
		set_message_timeout(other.get_message_timeout());
	}

	dul::Transport& Association::get_transport()
	{
		return m_stateMachine.get_transport();
	}

	Association& Association::operator=(const Association& other)
	{
		if (this != &other)
		{
			set_peer_host(other.get_peer_host());
			set_peer_port(other.get_peer_port());
			set_parameters(other.get_parameters());
		}

		return *this;
	}

	const std::string& Association::get_peer_host() const
	{
		return m_peerHost;
	}

	void Association::set_peer_host(const std::string& host)
	{
		if (is_associated())
		{
			throw Exception("Cannot set member while associated");
		}

		m_peerHost = host;
	}

	uint16_t Association::get_peer_port() const
	{
		return m_peerPort;
	}

	void Association::set_peer_port(uint16_t port)
	{
		if (is_associated())
		{
			throw Exception("Cannot set member while associated");
		}

		m_peerPort = port;
	}

	const odil::AssociationParameters& Association::get_parameters() const
	{
		return m_associationParameters;
	}

	odil::AssociationParameters& Association::update_parameters()
	{
		if (is_associated())
		{
			throw Exception("Cannot set member while associated");
		}

		return m_associationParameters;
	}

	void Association::set_parameters(const odil::AssociationParameters& value)
	{
		if (is_associated())
		{
			throw Exception("Cannot set member while associated");
		}

		m_associationParameters = value;
	}

	const odil::AssociationParameters& Association::get_negotiated_parameters() const
	{
		return m_negotiatedParameters;
	}

	Association::duration_type Association::get_tcp_timeout() const
	{
		return m_stateMachine.get_transport().get_timeout();
	}

	void Association::set_tcp_timeout(const duration_type& duration)
	{
		m_stateMachine.get_transport().set_timeout(duration);
	}

	Association::duration_type Association::get_message_timeout() const
	{
		return m_stateMachine.get_timeout();
	}

	void Association::set_message_timeout(const duration_type& duration)
	{
		m_stateMachine.set_timeout(duration);
	}

	bool Association::is_associated() const
	{
		return m_stateMachine.get_transport().is_open() && m_stateMachine.get_state() == dul::StateMachine::State::Sta6;
	}

	void Association::associate()
	{
		boost::asio::ip::tcp::resolver resolver(
			m_stateMachine.get_transport().get_service());
		const boost::asio::ip::tcp::resolver::query query(m_peerHost, "");
		const auto endpoint_it = resolver.resolve(query);

		dul::EventData data;
		data.peer_endpoint = *endpoint_it;
		data.peer_endpoint.port(m_peerPort);

		const auto request = std::make_shared<odil::pdu::AAssociateRQ>(m_associationParameters.as_a_associate_rq());

		data.pdu = request;

		m_stateMachine.send_pdu(data);
		m_stateMachine.receive_pdu(data);

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

				_transfer_syntaxes_by_id.clear();

				for (const auto& pc : m_negotiatedParameters.get_presentation_contexts())
				{
					if (pc.result == odil::AssociationParameters::PresentationContext::Result::Acceptance)
					{
						_transfer_syntaxes_by_id[pc.id] = pc.transfer_syntaxes[0];
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

	void Association::receive_association(std::shared_ptr<dul::Transport::Socket> socket, odil::AssociationAcceptor acceptor)
	{
		dul::EventData data;
		data.socket = std::move(socket);

		m_stateMachine.set_association_acceptor(acceptor);

		m_stateMachine.receive(data);
		m_stateMachine.receive_pdu(data);

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
			{
				throw Exception("Invalid response");
			}

			const auto endpoint = m_stateMachine.get_transport().get_socket()->remote_endpoint();
			m_peerHost = endpoint.address().to_string();
			m_peerPort = endpoint.port();

			m_negotiatedParameters = data.association_parameters;

			_transfer_syntaxes_by_id.clear();

			for (const auto& pc : m_negotiatedParameters.get_presentation_contexts())
			{
				if (pc.result == odil::AssociationParameters::PresentationContext::Result::Acceptance)
				{
					_transfer_syntaxes_by_id[pc.id] = pc.transfer_syntaxes[0];
				}
			}

			data.pdu = std::make_shared<odil::pdu::AAssociateAC>(m_negotiatedParameters.as_a_associate_ac());
			m_stateMachine.send_pdu(data);
		}
	}

	void Association::receive_association(const boost::asio::ip::tcp& protocol, unsigned short port, odil::AssociationAcceptor acceptorFunc)
	{
		auto& service = get_transport().get_service();
		auto socket = std::make_shared<dul::Transport::Socket>(service);

		dul::Transport::Socket::endpoint_type endpoint{protocol, port};
		boost::asio::ip::tcp::acceptor acceptor{service, endpoint};
		boost::system::error_code ec;
		acceptor.accept(*socket, ec);

		receive_association(socket, acceptorFunc);
	}

	void Association::release()
	{
		if (!is_associated())
		{
			throw Exception("Not associated");
		}

		auto pdu = std::make_shared<odil::pdu::AReleaseRQ>();
		dul::EventData data;
		data.pdu = pdu;
		m_stateMachine.send_pdu(data);
		m_stateMachine.receive_pdu(data);

		const auto reply = std::dynamic_pointer_cast<odil::pdu::AReleaseRP>(data.pdu);
		if (!reply)
		{
			// Invalid response, accept it nevertheless.
		}
	}

	void Association::abort(int source, int reason)
	{
		if (!is_associated())
		{
			throw Exception("Not associated");
		}

		auto pdu = std::make_shared<odil::pdu::AAbort>(source, reason);
		dul::EventData data;
		data.pdu = pdu;
		m_stateMachine.send_pdu(data);
	}

	uint16_t Association::next_message_id()
	{
		return ++m_nextMessageId;
	}

	std::string Association::get_transfer_syntax_by_id(int presentation_context_id)
	{
		const auto transfer_syntax_it = _transfer_syntaxes_by_id.find(presentation_context_id);
		if (transfer_syntax_it == _transfer_syntaxes_by_id.end())
		{
			throw Exception("No such Presentation Context ID");
		}

		return transfer_syntax_it->second;
	}

	dul::StateMachine& Association::stateMachine()
	{
		return m_stateMachine;
	}
}
