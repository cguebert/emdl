#include <emdl/pdu/PDataTF.h>

namespace emdl
{
	namespace pdu
	{
		PDataTF::PDataTF()
			: pdvItems(initField("Presentation-data-value-Items"), length, 0)
		{
			pduType.set(PDUType::P_DATA_TF);
		}

		PDataTF::PDataTF(const std::vector<PresentationDataValue>& list)
			: PDataTF()
		{
			pdvItems.set(list);
		}

		PDataTF::PDataTF(std::istream& in)
			: PDataTF()
		{
			read(in);
		}
	}
}
