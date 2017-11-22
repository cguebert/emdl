#include <emdl/pdu/items/PresentationContextAC.h>
#include <emdl/pdu/items/ItemFactory.h>
#include <emdl/pdu/subitems/TransferSyntaxSubItem.h>

namespace emdl
{
	namespace pdu
	{
		PresentationContextAC::PresentationContextAC()
		{
			itemType.set(ItemType::PresentationContextAC);
		}
		PresentationContextAC::PresentationContextAC(uint8_t id, TransferSyntax transferSyntax, uint8_t res)
			: PresentationContextAC()
		{
			presentationContextId.set(id);
			result.set(res);
			setTransferSyntax(transferSyntax);
		}

		PresentationContextAC::PresentationContextAC(std::istream& in)
			: PresentationContextAC()
		{
			read(in);
		}

		TransferSyntax PresentationContextAC::transferSyntax() const
		{
			const auto tsItems = subItems.get<TransferSyntaxSubItem>();
			if (tsItems.empty())
				throw Exception("No Transfer Syntax sub-item");
			else if (tsItems.size() > 1)
				throw Exception("Several Transfer Syntax sub-items");

			return getTransferSyntax(tsItems.front()->name.get());
		}

		void PresentationContextAC::setTransferSyntax(TransferSyntax ts)
		{
			subItems.remove<TransferSyntaxSubItem>();
			subItems.add(std::make_shared<TransferSyntaxSubItem>(ts));
		}

		RegisterItem<PresentationContextAC> presentationContextACItem{ItemType::PresentationContextAC};
	}
}
