#include <emdl/pdu/subitems/AbstractSyntaxSubItem.h>
#include <emdl/pdu/items/ItemFactory.h>

namespace emdl
{
	namespace pdu
	{
		AbstractSyntaxSubItem::AbstractSyntaxSubItem()
			: name(initField("Abstract-syntax-name"), length)
		{
			itemType.set(ItemType::AbstractSyntax);
		}

		AbstractSyntaxSubItem::AbstractSyntaxSubItem(SOP_Class sop)
			: AbstractSyntaxSubItem()
		{
			name.set(getSOPClassUID(sop));
		}

		AbstractSyntaxSubItem::AbstractSyntaxSubItem(std::istream& in)
			: AbstractSyntaxSubItem()
		{
			read(in);
		}

		RegisterItem<AbstractSyntaxSubItem> abstractSyntaxSubItem{ItemType::AbstractSyntax};
	}
}
