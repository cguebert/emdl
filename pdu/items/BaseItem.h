#pragma once

#include <emdl/pdu/Object.h>

namespace emdl
{
	namespace pdu
	{
		struct ItemType
		{
			enum Type
			{
				ApplicationContext = 0x10,
				PresentationContextRQ = 0x20,
				PresentationContextAC = 0x21,
				AbstractSyntax = 0x30,
				TransferSyntax = 0x40,
				UserInformation = 0x50
			};
		};

		//! Base class for all PDU items
		class EMDL_API BaseItem : public Object
		{
		public:
			BaseItem();

			void read(std::istream& in) override;
			void save(std::ostream& out) const override;

			uint8_t type() const;

		protected:
			Field<uint8_t> itemType;
			Field<uint8_t> reserved1;
			mutable Field<uint16_t> length;
		};
	}
}
