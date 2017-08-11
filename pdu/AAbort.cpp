#include <emdl/pdu/AAbort.h>

namespace emdl
{
	namespace pdu
	{
		AAbort::AAbort()
			: reserved2(initField("Reserved-2", 0))
			, reserved3(initField("Reserved-3", 0))
			, source(initField("Source", 0))
			, reason(initField("Reason", 0))
		{
			pduType.set(PDUType::A_ABORT);
			length.set(4);
		}

		AAbort::AAbort(uint8_t s, uint8_t r)
			: AAbort()
		{
			source.set(s);
			reason.set(r);
		}

		AAbort::AAbort(std::istream& in)
			: AAbort()
		{
			read(in);
		}
	}
}
