#include <emdl/pdu/subitems/ImplementationClassUID.h>
#include <emdl/pdu/items/ItemFactory.h>

namespace emdl
{
	namespace pdu
	{
		ImplementationClassUID::ImplementationClassUID()
			: implementationClassUid(initField("Implementation-class-uid"), length, 0)
		{
			itemType.set(ItemType::ImplementationClassUID);
		}

		ImplementationClassUID::ImplementationClassUID(const std::string& uid)
			: ImplementationClassUID()
		{
			implementationClassUid.set(uid);
		}

		ImplementationClassUID::ImplementationClassUID(std::istream& in)
			: ImplementationClassUID()
		{
			read(in);
		}

		RegisterItem<ImplementationClassUID> implementationClassUIDSubItem{ItemType::ImplementationClassUID};
	}
}
