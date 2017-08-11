#include <emdl/pdu/BasePDU.h>

namespace emdl
{
	namespace pdu
	{
		BasePDU::BasePDU()
			: pduType(initField("PDU-type"))
			, reserved1(initField("Reserved-1", 0))
			, length(initField("PDU-length", 0))
		{
		}
	}
}
