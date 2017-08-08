#pragma once

#include <memory>

#include <emdl/emdl_api.h>
#include <emdl/dul/Transport.h>

#include <boost/optional.hpp>

#include <odil/AssociationAcceptor.h>
#include <odil/AssociationParameters.h>
#include <odil/pdu/Object.h>

namespace emdl
{
	namespace dul
	{
		/// Data related to events of the DUL state machine.
		class EMDL_API EventData
		{
		public:
			std::shared_ptr<odil::pdu::Object> pdu;
			boost::optional<Transport::Socket::endpoint_type> endpoint;
			boost::optional<odil::AssociationParameters> associationParameters;
			boost::optional<odil::AssociationRejected> reject;
		};
	}
}
