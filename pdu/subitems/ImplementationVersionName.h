#pragma once

#include <emdl/pdu/items/BaseItem.h>
#include <emdl/pdu/ObjectStringField.h>

namespace emdl
{
	namespace pdu
	{
		/// Implementation Version Name Sub-Item (PS 3.7, D.3.3.2.3 and D.3.3.2.4).
		class EMDL_API ImplementationVersionName : public BaseItem
		{
		public:
			ImplementationVersionName(const std::string& implementationVersionName);
			ImplementationVersionName(std::istream& in);

			StringField<uint16_t> implementationVersionName;

		private:
			ImplementationVersionName();
		};
	}
}
