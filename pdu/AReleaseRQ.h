#pragma once

#include <emdl/pdu/BasePDU.h>

namespace emdl
{
	namespace pdu
	{
		/// A-RELEASE-RP PDU, cf. PS 3.8, 9.3.6.
		class EMDL_API AReleaseRQ : public BasePDU
		{
		public:
			AReleaseRQ();
			AReleaseRQ(std::istream& in);

			const Field<uint32_t> reserved2;
		};
	}
}
