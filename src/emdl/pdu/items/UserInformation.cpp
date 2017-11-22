#include <emdl/pdu/items/UserInformation.h>
#include <emdl/pdu/items/ItemFactory.h>

namespace emdl
{
	namespace pdu
	{
		UserInformation::UserInformation()
			: subItems(initField("User-data"), length, 0)
		{
			itemType.set(ItemType::UserInformation);
		}

		UserInformation::UserInformation(std::istream& in)
			: UserInformation()
		{
			read(in);
		}

		RegisterItem<UserInformation> userInformationItem{ItemType::UserInformation};
	}
}
