#pragma once

#include <emdl/pdu/items/BaseItem.h>
#include <emdl/pdu/ObjectStringField.h>
#include <emdl/SOPClasses.h>

namespace emdl
{
	namespace pdu
	{
		class EMDL_API AbstractSyntaxSubItem : public BaseItem
		{
		public:
			AbstractSyntaxSubItem(SOP_Class sop);
			AbstractSyntaxSubItem(std::istream& in);

			StringField<uint16_t> name;

		private:
			AbstractSyntaxSubItem();
		};
	}
}
