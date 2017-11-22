#pragma once

#include <memory>

#include <emdl/association/AssociationAcceptor.h>
#include <emdl/dul/Transport.h>
#include <emdl/pdu/BasePDU.h>

#include <boost/optional.hpp>

namespace emdl
{
	namespace dul
	{
		/// Data related to events of the DUL state machine.
		class EMDL_API EventData
		{
		public:
			std::shared_ptr<pdu::BasePDU> pdu;
			boost::optional<Transport::Socket::endpoint_type> endpoint;
			boost::optional<AssociationParameters> associationParameters;
			boost::optional<AssociationRejected> reject;
		};
	}
}
