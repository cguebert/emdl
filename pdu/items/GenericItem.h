#pragma once

#include <emdl/pdu/items/BaseItem.h>
#include <emdl/pdu/ObjectStringField.h>

namespace emdl
{
	namespace pdu
	{
		class EMDL_API GenericItem : public BaseItem
		{
		public:
			GenericItem(std::istream& in);

			StringField<uint16_t> data;

		private:
			GenericItem();
		};
	}
}
