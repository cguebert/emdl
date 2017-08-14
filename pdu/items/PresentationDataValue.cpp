#include <emdl/pdu/items/PresentationDataValue.h>

namespace emdl
{
	namespace pdu
	{
		PresentationDataValue::PresentationDataValue()
			: presentationContextId(initField("Presentation-Context-ID", 0))
			, controlHeader(initField("Control-header", 0))
			, fragment(initField("Fragment"), length, 2)
		{
		}

		PresentationDataValue::PresentationDataValue(uint8_t pcId, uint8_t header, const std::string& frag)
			: PresentationDataValue()
		{
			if (pcId % 2 == 0)
				throw Exception("Invalid Presentation Context ID");

			presentationContextId.set(pcId);
			controlHeader.set(header);
			fragment.set(frag);
		}

		PresentationDataValue::PresentationDataValue(std::istream& in)
			: PresentationDataValue()
		{
			read(in);
		}

		bool PresentationDataValue::isCommand() const
		{
			return (controlHeader.get() & 0x01) != 0;
		}

		bool PresentationDataValue::isLastFragment() const
		{
			return (controlHeader.get() & 0x02) != 0;
		}
	}
}
