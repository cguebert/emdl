#pragma once

#include <emdl/pdu/Object.h>

namespace emdl
{
	namespace pdu
	{
		enum class PDUType : uint8_t
		{
			A_ASSOCIATE_RQ = 0x01,
			A_ASSOCIATE_AC = 0x02,
			A_ASSOCIATE_RJ = 0x03,
			P_DATA_TF = 0x04,
			A_RELEASE_RQ = 0x05,
			A_RELEASE_RP = 0x06,
			A_ABORT = 0x07
		};

		//! Base class for all PDUs
		class BasePDU : public Object
		{
		public:
			BasePDU();

			Field<uint8_t> pduType;
			Field<uint8_t> reserved1;
			Field<uint32_t> length;
		};
	}
}
