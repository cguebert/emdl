#include <emdl/pdu/subitems/SOPClassCommonExtendedNegotiation.h>
#include <emdl/pdu/items/ItemFactory.h>

namespace emdl
{
	namespace pdu
	{
		SOPClassCommonExtendedNegotiation::SOPClassCommonExtendedNegotiation()
			: m_sopClassUidLength(initField("SOP-class-uid-length", 0))
			, m_sopClassUid(initField("SOP-class-uid"), m_sopClassUidLength, 0)
			, m_serviceClassUidLength(initField("Service-class-uid-length", 0))
			, m_serviceClassUid(initField("Service-class-uid"), m_serviceClassUidLength, 0)
			, m_relatedSopClassUidsLength(initField("Related-general-sop-class-identification-length", 0))
			, m_relatedSopClassUids(initField("Related-general-sop-class-identification"), m_relatedSopClassUidsLength, 0)
		{
			itemType.set(ItemType::SOPClassCommonExtendedNegotiation);
		}

		SOPClassCommonExtendedNegotiation::SOPClassCommonExtendedNegotiation(
			const std::string& sopUid,
			const std::string& serviceUid,
			const std::vector<std::string>& relatedUids)
			: SOPClassCommonExtendedNegotiation()
		{
			m_sopClassUid.set(sopUid);
			m_serviceClassUid.set(serviceUid);

			setRelatedGeneralSopClassUids(relatedUids);
		}

		SOPClassCommonExtendedNegotiation::SOPClassCommonExtendedNegotiation(std::istream& in)
			: SOPClassCommonExtendedNegotiation()
		{
			read(in);
		}

		uint8_t SOPClassCommonExtendedNegotiation::version() const
		{
			return reserved1.get();
		}

		const std::string& SOPClassCommonExtendedNegotiation::sopClassUid() const
		{
			return m_sopClassUid.get();
		}

		void SOPClassCommonExtendedNegotiation::setSopClassUid(const std::string& uid)
		{
			m_sopClassUid.set(uid);
			length.set(Object::size());
		}

		const std::string& SOPClassCommonExtendedNegotiation::serviceClassUid() const
		{
			return m_serviceClassUid.get();
		}

		void SOPClassCommonExtendedNegotiation::setServiceClassUid(const std::string& uid)
		{
			m_serviceClassUid.set(uid);
			length.set(Object::size());
		}

		std::vector<std::string> SOPClassCommonExtendedNegotiation::relatedGeneralSopClassUids() const
		{
			std::vector<std::string> list;
			for (const auto& subField : m_relatedSopClassUids.get())
				list.push_back(subField.uid());
			return list;
		}

		void SOPClassCommonExtendedNegotiation::setRelatedGeneralSopClassUids(const std::vector<std::string>& uids)
		{
			std::vector<RelatedGeneralSOPClassIdentification> list;
			for (const auto& uid : uids)
				list.emplace_back(uid);
			m_relatedSopClassUids.set(list);
			length.set(Object::size());
		}

		/*****************************************************************************/

		SOPClassCommonExtendedNegotiation::RelatedGeneralSOPClassIdentification::RelatedGeneralSOPClassIdentification()
			: m_length(initField("Related-general-sop-class-identification-length", 0))
			, m_uid(initField("Related-general-sop-class-identification"), m_length, 0)
		{
		}

		SOPClassCommonExtendedNegotiation::RelatedGeneralSOPClassIdentification::RelatedGeneralSOPClassIdentification(const std::string& id)
			: RelatedGeneralSOPClassIdentification()
		{
			m_uid.set(id);
		}

		SOPClassCommonExtendedNegotiation::RelatedGeneralSOPClassIdentification::RelatedGeneralSOPClassIdentification(std::istream& in)
			: RelatedGeneralSOPClassIdentification()
		{
			read(in);
		}

		SOPClassCommonExtendedNegotiation::RelatedGeneralSOPClassIdentification::RelatedGeneralSOPClassIdentification(RelatedGeneralSOPClassIdentification& other)
			: RelatedGeneralSOPClassIdentification()
		{
			m_uid.set(other.uid());
		}

		SOPClassCommonExtendedNegotiation::RelatedGeneralSOPClassIdentification& SOPClassCommonExtendedNegotiation::RelatedGeneralSOPClassIdentification::operator=(RelatedGeneralSOPClassIdentification& other)
		{
			m_uid.set(other.uid());
			return *this;
		}

		const std::string& SOPClassCommonExtendedNegotiation::RelatedGeneralSOPClassIdentification::uid() const
		{
			return m_uid.get();
		}

		RegisterItem<SOPClassCommonExtendedNegotiation> sopClassCommonExtendedNegotiationSubItem{ItemType::SOPClassCommonExtendedNegotiation};
	}
}
