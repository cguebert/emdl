#pragma once

#include <emdl/pdu/items/BaseItem.h>
#include <emdl/pdu/ObjectStringField.h>

namespace emdl
{
	namespace pdu
	{
		/// SOP Class Extended Negotiation sub-item (PS 3.7, D.3.3.5).
		class EMDL_API SOPClassExtendedNegotiation : public BaseItem
		{
		public:
			SOPClassExtendedNegotiation(const std::string& sopClassUid,
										const std::string& serviceClassApplicationInformation);
			SOPClassExtendedNegotiation(std::istream& in);

			const std::string& sopClassUid() const;
			void setSopClassUid(const std::string& value);

			const std::string& serviceClassApplicationInformation() const;
			void setServiceClassApplicationInformation(const std::string& value);

		private:
			SOPClassExtendedNegotiation();

			Field<uint16_t> m_sopClassUidLength;
			StringField<uint16_t> m_sopClassUid;
			DynamicStringField m_serviceClassApplicationInformation;
		};
	}
}
