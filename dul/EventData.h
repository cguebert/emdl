#pragma once

#include <memory>

#include <emdl/emdl_api.h>

#include <odil/AssociationAcceptor.h>
#include <odil/AssociationParameters.h>
#include <odil/pdu/Object.h>
#include <emdl/dul/Transport.h>

namespace emdl
{
	namespace dul
	{
		/// @brief Data related to events of the DUL state machine.
		class EMDL_API EventData
		{
		public:
			Transport::Socket::endpoint_type peer_endpoint;
			std::shared_ptr<odil::pdu::Object> pdu;
			odil::AssociationParameters association_parameters;
			std::shared_ptr<odil::AssociationRejected> reject;
		};
	}
}
