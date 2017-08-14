#include <emdl/pdu/items/GenericItem.h>

namespace emdl
{
	namespace pdu
	{
		GenericItem::GenericItem()
			: data(initField("User-data"), length)
		{
		}

		GenericItem::GenericItem(std::istream& in)
			: GenericItem()
		{
			Object::read(in); // Also read the item type
		}
	}
}
