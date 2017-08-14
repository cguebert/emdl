#include <emdl/pdu/items/ItemFactory.h>
#include <emdl/pdu/items/GenericItem.h>

namespace emdl
{
	namespace pdu
	{
		BaseItemCreator::~BaseItemCreator() = default;

		ItemFactory& ItemFactory::instance()
		{
			static ItemFactory instance;
			return instance;
		}

		std::shared_ptr<BaseItem> ItemFactory::create(uint8_t type, std::istream& in)
		{
			const auto& registry = instance().registry;
			if (!registry.count(type))
				return std::make_shared<GenericItem>(in);

			return registry.at(type)->create(in);
		}

		void ItemFactory::registerItem(uint8_t type, BaseItemCreator::SPtr creator)
		{
			registry[type] = creator;
		}
	}
}
