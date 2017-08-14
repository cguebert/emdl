#include <emdl/pdu/subitems/TransferSyntaxSubItem.h>
#include <emdl/pdu/items/ItemFactory.h>

namespace emdl
{
	namespace pdu
	{
		TransferSyntaxSubItem::TransferSyntaxSubItem()
			: name(initField("Transfer-syntax-name"), length)
		{
			itemType.set(ItemType::TransferSyntax);
		}

		TransferSyntaxSubItem::TransferSyntaxSubItem(TransferSyntax ts)
			: TransferSyntaxSubItem()
		{
			name.set(getTransferSyntaxUID(ts));
		}

		TransferSyntaxSubItem::TransferSyntaxSubItem(std::istream& in)
			: TransferSyntaxSubItem()
		{
			read(in);
		}

		RegisterItem<TransferSyntaxSubItem> trasnferSyntaxSubItem{ItemType::TransferSyntax};
	}
}
