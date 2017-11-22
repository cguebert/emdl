#include <emdl/pdu/items/PresentationContextRQ.h>
#include <emdl/pdu/items/ItemFactory.h>
#include <emdl/pdu/subitems/AbstractSyntaxSubItem.h>
#include <emdl/pdu/subitems/TransferSyntaxSubItem.h>

namespace emdl
{
	namespace pdu
	{
		PresentationContextRQ::PresentationContextRQ()
		{
			itemType.set(ItemType::PresentationContextRQ);
		}

		PresentationContextRQ::PresentationContextRQ(uint8_t id, SOP_Class abstractSyntax, const TransferSyntaxes& transferSyntaxes)
			: PresentationContextRQ()
		{
			presentationContextId.set(id);
			setAbstractSyntax(abstractSyntax);
			setTransferSyntaxes(transferSyntaxes);
		}

		PresentationContextRQ::PresentationContextRQ(std::istream& in)
			: PresentationContextRQ()
		{
			read(in);
		}

		SOP_Class PresentationContextRQ::abstractSyntax() const
		{
			const auto sopItems = subItems.get<AbstractSyntaxSubItem>();
			if (sopItems.empty())
				throw Exception("No Abstract Syntax sub-item");
			else if (sopItems.size() > 1)
				throw Exception("Several Abstract Syntax sub-items");

			return getSOPClass(sopItems.front()->name.get());
		}

		void PresentationContextRQ::setAbstractSyntax(SOP_Class abstractSyntax)
		{
			subItems.remove<AbstractSyntaxSubItem>();
			subItems.add(std::make_shared<AbstractSyntaxSubItem>(abstractSyntax));
		}

		std::vector<TransferSyntax> PresentationContextRQ::transferSyntaxes() const
		{
			const auto tsItems = subItems.get<TransferSyntaxSubItem>();
			std::vector<TransferSyntax> tsList;
			for (const auto& item : tsItems)
				tsList.push_back(getTransferSyntax(item->name.get()));
			return tsList;
		}

		void PresentationContextRQ::setTransferSyntaxes(const TransferSyntaxes& transferSyntaxes)
		{
			subItems.remove<TransferSyntaxSubItem>();
			std::vector<TransferSyntaxSubItem> itemsList;
			for (const auto ts : transferSyntaxes)
				subItems.add(std::make_shared<TransferSyntaxSubItem>(ts));
		}

		RegisterItem<PresentationContextRQ> presentationContextRQItem{ItemType::PresentationContextRQ};
	}
}
