#pragma once

#include <emdl/pdu/items/BaseItem.h>
#include <emdl/pdu/items/ObjectsField.h>
#include <emdl/pdu/ObjectStringField.h>

namespace emdl
{
	namespace pdu
	{
		/// SOP Class Common Extended Negotiation sub-item (PS 3.7, D.3.3.6).
		class EMDL_API SOPClassCommonExtendedNegotiation : public BaseItem
		{
		public:
			SOPClassCommonExtendedNegotiation(const std::string& sopClassUid,
											  const std::string& serviceClassUid = "",
											  const std::vector<std::string>& relatedGeneralSopClassUids = {});
			SOPClassCommonExtendedNegotiation(std::istream& in);

			uint8_t version() const;

			const std::string& sopClassUid() const;
			void setSopClassUid(const std::string& uid);

			const std::string& serviceClassUid() const;
			void setServiceClassUid(const std::string& uid);

			std::vector<std::string> relatedGeneralSopClassUids() const;
			void setRelatedGeneralSopClassUids(const std::vector<std::string>& uids);

		private:
			class RelatedGeneralSOPClassIdentification : public Object
			{
			public:
				RelatedGeneralSOPClassIdentification(const std::string& uid);
				RelatedGeneralSOPClassIdentification(std::istream& in);
				RelatedGeneralSOPClassIdentification(const RelatedGeneralSOPClassIdentification& other);

				RelatedGeneralSOPClassIdentification& operator=(const RelatedGeneralSOPClassIdentification& other);

				const std::string& uid() const;

			private:
				Field<uint16_t> m_length;
				StringField<uint16_t> m_uid;
				RelatedGeneralSOPClassIdentification();
			};

			Field<uint16_t> m_sopClassUidLength;
			StringField<uint16_t> m_sopClassUid;
			Field<uint16_t> m_serviceClassUidLength;
			StringField<uint16_t> m_serviceClassUid;
			Field<uint16_t> m_relatedSopClassUidsLength;
			ObjectsField<RelatedGeneralSOPClassIdentification, uint16_t> m_relatedSopClassUids;

			SOPClassCommonExtendedNegotiation();
		};
	}
}
