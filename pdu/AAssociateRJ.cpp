#include <emdl/pdu/AAssociateRJ.h>

namespace emdl
{
	namespace pdu
	{
		AAssociateRJ::AAssociateRJ()
			: reserved2(initField("Reserved-2", 0))
			, result(initField("Result", 0))
			, source(initField("Source", 0))
			, reason(initField("Reason", 0))
		{
			pduType.set(PDUType::A_ASSOCIATE_RJ);
		}

		AAssociateRJ::AAssociateRJ(uint8_t res, uint8_t s, uint8_t rea)
			: AAssociateRJ()
		{
			result.set(res);
			source.set(s);
			reason.set(rea);
		}

		AAssociateRJ::AAssociateRJ(std::istream& in)
			: AAssociateRJ()
		{
			read(in);
		}
	}
}
