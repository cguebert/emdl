#include <emdl/pdu/subitems/RoleSelection.h>

namespace emdl
{
	namespace pdu
	{
		RoleSelection::RoleSelection()
			: sopClassUid(initField("SOP-class-uid"), length, 0)
			, scuRole(initField("SCU-role", 0))
			, scpRole(initField("SCP-role", 0))
		{
			itemType.set(ItemType::RoleSelection);
		}

		RoleSelection ::RoleSelection(const std::string& uid, bool scu, bool scp)
			: RoleSelection()
		{
			sopClassUid.set(uid);
			scuRole.set(scu);
			scpRole.set(scp);
		}

		RoleSelection::RoleSelection(std::istream& in)
			: RoleSelection()
		{
			read(in);
		}

		bool RoleSelection::scuRoleSupport() const
		{
			return scuRole.get() != 0;
		}

		void RoleSelection::setScuRoleSupport(bool value)
		{
			scuRole.set(value ? 1 : 0);
		}

		bool RoleSelection::scpRoleSupport() const
		{
			return scpRole.get() != 0;
		}

		void RoleSelection::setScpRoleSupport(bool value)
		{
			scpRole.set(value ? 1 : 0);
		}
	}
}
