#pragma once

#include <emdl/pdu/BasePDU.h>

namespace emdl
{
	namespace pdu
	{
		/// A-RELEASE-RP PDU, cf. PS 3.8, 9.3.7.
		class EMDL_API AReleaseRP : public BasePDU
		{
		public:
			AReleaseRP();
			AReleaseRP(std::istream& in);

			const Field<uint32_t> reserved2;
		};
	}
}
