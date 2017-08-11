#pragma once

#include <emdl/pdu/BasePDU.h>

namespace emdl
{
	namespace pdu
	{
		/// A-ABORT PDU, cf. PS 3.8, 9.3.8.
		class EMDL_API AAbort : public BasePDU
		{
		public:
			AAbort(uint8_t source, uint8_t reason);
			AAbort(std::istream& in);

			Field<uint8_t> reserved2;
			Field<uint8_t> reserved3;
			Field<uint8_t> source;
			Field<uint8_t> reason;

		private:
			AAbort();
		};
	}
}
