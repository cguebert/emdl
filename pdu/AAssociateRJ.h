#pragma once

#include <emdl/pdu/BasePDU.h>

namespace emdl
{
	namespace pdu
	{
		/// A-ASSOCIATE-RJ PDU, cf. PS 3.8, 9.3.4.
		class EMDL_API AAssociateRJ : public BasePDU
		{
		public:
			AAssociateRJ(uint8_t result, uint8_t source, uint8_t reason);
			AAssociateRJ(std::istream& in);

			const Field<uint8_t> reserved2;
			Field<uint8_t> result;
			Field<uint8_t> source;
			Field<uint8_t> reason;

		private:
			AAssociateRJ();
		};
	}
}
