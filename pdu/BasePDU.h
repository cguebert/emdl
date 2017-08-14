#pragma once

#include <emdl/pdu/Object.h>

namespace emdl
{
	namespace pdu
	{
		struct PDUType
		{
			enum Type
			{
				A_ASSOCIATE_RQ = 0x01,
				A_ASSOCIATE_AC = 0x02,
				A_ASSOCIATE_RJ = 0x03,
				P_DATA_TF = 0x04,
				A_RELEASE_RQ = 0x05,
				A_RELEASE_RP = 0x06,
				A_ABORT = 0x07
			};
		};

		//! Base class for all PDUs
		class BasePDU : public Object
		{
		public:
			BasePDU();

			void read(std::istream& in) override;
			void save(std::ostream& out) const override;

		protected:
			Field<uint8_t> pduType;
			Field<uint8_t> reserved1;
			mutable Field<uint32_t> length;
		};
	}
}
