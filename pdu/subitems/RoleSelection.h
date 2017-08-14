#pragma once

#include <emdl/pdu/items/BaseItem.h>
#include <emdl/pdu/ObjectStringField.h>

namespace emdl
{
	namespace pdu
	{
		/// SCU/SCP Role Selection Sub-Item (PS 3.7, D.3.3.4.1 and D.3.3.4.2).
		class EMDL_API RoleSelection : public BaseItem
		{
		public:
			RoleSelection(const std::string& sopClassUid, bool scuRoleSupport, bool scpRoleSupport);
			RoleSelection(std::istream& in);

			StringField<uint16_t> sopClassUid;

			bool scuRoleSupport() const;
			void setScuRoleSupport(bool value);

			bool scpRoleSupport() const;
			void setScpRoleSupport(bool value);

		private:
			Field<uint8_t> scuRole;
			Field<uint8_t> scpRole;

			RoleSelection();
		};
	}
}
