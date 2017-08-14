#pragma once

#include <emdl/pdu/items/BaseItem.h>
#include <emdl/pdu/ObjectStringField.h>

namespace emdl
{
	namespace pdu
	{
		///  Presentation Data Value Item, (PS 3.8, 9.3.5.1).
		class EMDL_API PresentationDataValue : public BaseItem
		{
		public:
			PresentationDataValue(uint8_t presentationContextId, uint8_t controlHeader, const std::string& fragment);
			PresentationDataValue(std::istream& in);

			bool isCommand() const;
			bool isLastFragment() const;

			Field<uint8_t> presentationContextId;
			Field<uint8_t> controlHeader;
			StringField<uint16_t> fragment;

		private:
			PresentationDataValue();
		};
	}
}
