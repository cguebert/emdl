#include <emdl/pdu/AReleaseRQ.h>

namespace emdl
{
	namespace pdu
	{
		AReleaseRQ::AReleaseRQ()
			: reserved2(initField("Reserved-2", 0))
		{
			pduType.set(PDUType::A_RELEASE_RQ);
			length.set(4);
		}

		AReleaseRQ::AReleaseRQ(std::istream& in)
			: AReleaseRQ()
		{
			read(in);
		}
	}
}
