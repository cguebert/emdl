#pragma once

#include <emdl/pdu/items/BaseItem.h>
#include <emdl/pdu/items/ItemsField.h>

namespace emdl
{
	namespace pdu
	{
		/// Presentation Context item, either for a A-ASSOCIATE-RQ PDU (PS 3.8, 9.3.2.2) or for a A-ASSOCIATE-AC PDU (PS 3.8, 9.3.3.2).
		class EMDL_API PresentationContext : public BaseItem
		{
		public:
			PresentationContext();

			Field<uint8_t> presentationContextId;
			const Field<uint8_t> reserved2;
			Field<uint8_t> result;
			const Field<uint8_t> reserved3;

		protected:
			ItemsField<uint16_t> subItems;
		};
	}
}
