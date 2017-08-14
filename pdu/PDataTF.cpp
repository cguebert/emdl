#include <emdl/pdu/PDataTF.h>

namespace emdl
{
	namespace pdu
	{
		PDataTF::PDataTF()
			: items(initField("Presentation-data-value-Items"), length, 0)
		{
			pduType.set(PDUType::P_DATA_TF);
		}

		PDataTF::PDataTF(const std::vector<PresentationDataValueSPtr>& pdvItems)
			: PDataTF()
		{
			items.add(pdvItems);
		}

		PDataTF::PDataTF(std::istream& in)
			: PDataTF()
		{
			read(in);
		}

		const std::vector<PresentationDataValueSPtr> PDataTF::pdvItems() const
		{
			return items.get<PresentationDataValue>();
		}
	}
}
