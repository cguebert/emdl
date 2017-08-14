#include <emdl/pdu/PDataTF.h>

namespace emdl
{
	namespace pdu
	{
		PDataTF::PDataTF()
			: items(initField("Presentation-data-value-Items"), length, 0)
		{
		}

		PDataTF::PDataTF(const std::vector<PresentationDataValueSPtr> pdvItems)
			: PDataTF()
		{
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
