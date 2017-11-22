#pragma once

#include <emdl/pdu/items/BaseItem.h>
#include <emdl/pdu/ObjectStringField.h>

namespace emdl
{
	namespace pdu
	{
		/// Implementation Class UID Sub-Item (PS 3.7, D.3.3.2.1 and D.3.3.2.2).
		class EMDL_API ImplementationClassUID : public BaseItem
		{
		public:
			ImplementationClassUID(const std::string& implementationClassUid);
			ImplementationClassUID(std::istream& in);

			StringField<uint16_t> implementationClassUid;

		private:
			ImplementationClassUID();
		};
	}
}
