#pragma once

#include <emdl/pdu/items/BaseItem.h>
#include <emdl/pdu/ObjectStringField.h>
#include <emdl/SOPClasses.h>

namespace emdl
{
	namespace pdu
	{
		/// SCU/SCP Role Selection Sub-Item (PS 3.7, D.3.3.4.1 and D.3.3.4.2).
		class EMDL_API RoleSelection : public BaseItem
		{
		public:
			RoleSelection(SOP_Class sopClassUid, bool scuRoleSupport, bool scpRoleSupport);
			RoleSelection(std::istream& in);

			SOP_Class sopClass() const;
			void setSopClass(SOP_Class sopClass);

			bool scuRoleSupport() const;
			void setScuRoleSupport(bool value);

			bool scpRoleSupport() const;
			void setScpRoleSupport(bool value);

		private:
			Field<uint16_t> m_uidLength;
			StringField<uint16_t> m_sopClassUid;
			Field<uint8_t> m_scuRole;
			Field<uint8_t> m_scpRole;

			RoleSelection();
		};
	}
}
