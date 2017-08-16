#include <emdl/pdu/items/PresentationDataValue.h>

namespace emdl
{
	namespace pdu
	{
		PresentationDataValue::PresentationDataValue()
			: m_itemLength(initField("Item-length", 0))
			, m_presentationContextId(initField("Presentation-Context-ID", 0))
			, m_controlHeader(initField("Control-header", 0))
			, m_fragment(initField("Fragment"), m_itemLength, 2)
		{
		}

		PresentationDataValue::PresentationDataValue(uint8_t presentationContextId, uint8_t controlHeader, const std::string& fragment)
			: PresentationDataValue()
		{
			if (presentationContextId % 2 == 0)
				throw Exception("Invalid Presentation Context ID");

			m_presentationContextId.set(presentationContextId);
			m_controlHeader.set(controlHeader);
			m_fragment.set(fragment);
		}

		PresentationDataValue::PresentationDataValue(std::istream& in)
			: PresentationDataValue()
		{
			read(in);
		}

		PresentationDataValue::PresentationDataValue(const PresentationDataValue& other)
			: PresentationDataValue()
		{
			m_presentationContextId.set(other.m_presentationContextId.get());
			m_controlHeader.set(other.m_controlHeader.get());
			m_fragment.set(other.m_fragment.get());
		}

		PresentationDataValue& PresentationDataValue::operator=(const PresentationDataValue& other)
		{
			m_presentationContextId.set(other.m_presentationContextId.get());
			m_controlHeader.set(other.m_controlHeader.get());
			m_fragment.set(other.m_fragment.get());
			return *this;
		}

		uint8_t PresentationDataValue::presentationContextId() const
		{
			return m_presentationContextId.get();
		}

		bool PresentationDataValue::isCommand() const
		{
			return (m_controlHeader.get() & 0x01) != 0;
		}

		bool PresentationDataValue::isLastFragment() const
		{
			return (m_controlHeader.get() & 0x02) != 0;
		}

		const std::string& PresentationDataValue::fragment() const
		{
			return m_fragment.get();
		}
	}
}
