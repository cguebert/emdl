#pragma once

#include <emdl/pdu/items/BaseItem.h>
#include <emdl/pdu/ObjectStringField.h>

namespace emdl
{
	namespace pdu
	{
		/// Application Context item, (PS 3.8, 9.3.2.1).
		class EMDL_API ApplicationContext : public BaseItem
		{
		public:
			ApplicationContext(const std::string& name);
			ApplicationContext(std::istream& in);

			StringField<uint16_t> name;

		private:
			ApplicationContext();
		};
	}
}
