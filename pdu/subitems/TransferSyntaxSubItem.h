#pragma once

#include <emdl/pdu/items/BaseItem.h>
#include <emdl/pdu/ObjectStringField.h>
#include <emdl/TransferSyntaxes.h>

namespace emdl
{
	namespace pdu
	{
		class EMDL_API TransferSyntaxSubItem : public BaseItem
		{
		public:
			TransferSyntaxSubItem(TransferSyntax ts);
			TransferSyntaxSubItem(std::istream& in);

			StringField<uint16_t> name;

		private:
			TransferSyntaxSubItem();
		};
	}
}
