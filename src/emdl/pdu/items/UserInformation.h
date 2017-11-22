#pragma once

#include <emdl/pdu/items/BaseItem.h>
#include <emdl/pdu/items/ItemsField.h>

namespace emdl
{
	namespace pdu
	{
		/// User Information Item Structure (PS 3.8, 9.3.2.3 and 9.3.3.3).
		class EMDL_API UserInformation : public BaseItem
		{
		public:
			UserInformation();
			UserInformation(std::istream& in);

			ItemsField<uint16_t> subItems;
		};
	}
}
