#include <emdl/pdu/subitems/SOPClassExtendedNegotiation.h>

namespace emdl
{
	namespace pdu
	{
		SOPClassExtendedNegotiation::SOPClassExtendedNegotiation()
			: m_sopClassUidLength(initField("SOP-class-uid-length", 0))
			, m_sopClassUid(initField("SOP-class-uid"), m_sopClassUidLength, 0)
			, m_serviceClassApplicationInformation(initField("Service-class-application-information"),
												   [this]() {
													   return length.get()
															  - m_sopClassUidLength.get()
															  - 2;
												   },
												   [this](size_t size) {
													   length.set(m_sopClassUidLength.get()
																  + static_cast<uint16_t>(size)
																  + 2);
												   })
		{
			itemType.set(ItemType::SOPClassExtendedNegotiation);
		}

		SOPClassExtendedNegotiation::SOPClassExtendedNegotiation(const std::string& sopUid, const std::string& serviceInfo)
			: SOPClassExtendedNegotiation()
		{
			m_sopClassUid.set(sopUid);
		}

		SOPClassExtendedNegotiation::SOPClassExtendedNegotiation(std::istream& in)
			: SOPClassExtendedNegotiation()
		{
			read(in);
		}

		const std::string& SOPClassExtendedNegotiation::sopClassUid() const
		{
			return m_sopClassUid.get();
		}

		void SOPClassExtendedNegotiation::setSopClassUid(const std::string& value)
		{
			m_sopClassUid.set(value);
		}

		const std::string& SOPClassExtendedNegotiation::serviceClassApplicationInformation() const
		{
			return m_serviceClassApplicationInformation.get();
		}

		void SOPClassExtendedNegotiation::setServiceClassApplicationInformation(const std::string& value)
		{
			m_serviceClassApplicationInformation.set(value);
		}
	}
}
