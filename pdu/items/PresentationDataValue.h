#pragma once

#include <emdl/pdu/Object.h>
#include <emdl/pdu/ObjectStringField.h>

namespace emdl
{
	namespace pdu
	{
		///  Presentation Data Value Item, (PS 3.8, 9.3.5.1).
		class EMDL_API PresentationDataValue : public Object
		{
		public:
			PresentationDataValue(uint8_t presentationContextId, uint8_t controlHeader, const std::string& fragment);
			PresentationDataValue(std::istream& in);
			PresentationDataValue(const PresentationDataValue& other);

			PresentationDataValue& operator=(const PresentationDataValue& other);

			uint8_t presentationContextId() const;

			bool isCommand() const;
			bool isLastFragment() const;

			const std::string& fragment() const;

		private:
			PresentationDataValue();

			Field<uint32_t> m_itemLength;
			Field<uint8_t> m_presentationContextId;
			Field<uint8_t> m_controlHeader;
			StringField<uint32_t> m_fragment;
		};
	}
}
