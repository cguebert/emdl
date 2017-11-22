#include <emdl/pdu/AReleaseRP.h>

namespace emdl
{
	namespace pdu
	{
		AReleaseRP::AReleaseRP()
			: reserved2(initField("Reserved-2", 0))
		{
			pduType.set(PDUType::A_RELEASE_RP);
			length.set(4);
		}

		AReleaseRP::AReleaseRP(std::istream& in)
			: AReleaseRP()
		{
			read(in);
		}
	}
}
