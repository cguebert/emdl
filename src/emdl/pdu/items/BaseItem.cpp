#include <emdl/pdu/items/BaseItem.h>
#include <emdl/Exception.h>

namespace emdl
{
	namespace pdu
	{
		BaseItem::BaseItem()
			: itemType(initField("Item-type"))
			, reserved1(initField("Reserved-1", 0))
			, length(initField("Item-length", 0))
		{
		}

		void BaseItem::read(std::istream& in)
		{
			uint8_t type = 0;
			in.read(reinterpret_cast<char*>(&type), 1);

			const auto expected = itemType.get();
			if (type != expected)
				throw Exception("Invalid Item type: excepted {}, got {}", expected, type);

			auto it = m_fields.begin(), itEnd = m_fields.end();
			std::advance(it, 1); // Ignore the type field
			for (; it != itEnd; ++it)
				(*it)->read(in);
		}

		void BaseItem::save(std::ostream& out) const
		{
			length.set(static_cast<uint16_t>(size()));
			Object::save(out);
		}

		uint8_t BaseItem::type() const
		{
			return itemType.get();
		}
	}
}
