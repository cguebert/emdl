#include <emdl/pdu/subitems/RoleSelection.h>

namespace emdl
{
	namespace pdu
	{
		RoleSelection::RoleSelection()
			: m_sopClassUid(initField("SOP-class-uid"), length, 0)
			, m_scuRole(initField("SCU-role", 0))
			, m_scpRole(initField("SCP-role", 0))
		{
			itemType.set(ItemType::RoleSelection);
		}

		RoleSelection::RoleSelection(SOP_Class sopClassUid, bool scu, bool scp)
			: RoleSelection()
		{
			m_sopClassUid.set(getSOPClassUID(sopClassUid));
			m_scuRole.set(scu);
			m_scpRole.set(scp);
		}

		RoleSelection::RoleSelection(std::istream& in)
			: RoleSelection()
		{
			read(in);
		}

		SOP_Class RoleSelection::sopClass() const
		{
			return getSOPClass(m_sopClassUid.get());
		}

		void RoleSelection::setSopClass(SOP_Class sopClass)
		{
			m_sopClassUid.set(getSOPClassUID(sopClass));
		}

		bool RoleSelection::scuRoleSupport() const
		{
			return m_scuRole.get() != 0;
		}

		void RoleSelection::setScuRoleSupport(bool value)
		{
			m_scuRole.set(value ? 1 : 0);
		}

		bool RoleSelection::scpRoleSupport() const
		{
			return m_scpRole.get() != 0;
		}

		void RoleSelection::setScpRoleSupport(bool value)
		{
			m_scpRole.set(value ? 1 : 0);
		}
	}
}
