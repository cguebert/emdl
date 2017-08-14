#pragma once

#include <emdl/pdu/items/BaseItem.h>

#include <istream>
#include <map>
#include <memory>

namespace emdl
{
	namespace pdu
	{
		class BaseItemCreator
		{
		public:
			using SPtr = std::shared_ptr<BaseItemCreator>;

			virtual ~BaseItemCreator();
			virtual std::shared_ptr<BaseItem> create(std::istream& in) const = 0;
		};

		class EMDL_API ItemFactory
		{
		public:
			static std::shared_ptr<BaseItem> create(uint8_t type, std::istream& in);

		private:
			ItemFactory() {}
			static ItemFactory& instance();

			using RegistryMap = std::map<uint8_t, BaseItemCreator::SPtr>;
			RegistryMap registry;

			template <class T>
			friend class RegisterItem;

			void registerItem(uint8_t type, BaseItemCreator::SPtr creator);
		};

		template <class T>
		class ItemCreator : public BaseItemCreator
		{
		public:
			std::shared_ptr<BaseItem> create(std::istream& in) const override
			{
				return std::make_shared<T>(in);
			}
		};

		template <class T>
		class RegisterItem
		{
		public:
			explicit RegisterItem(uint8_t type)
			{
				ItemFactory::instance().registerItem(type, std::make_shared<ItemCreator<T>>());
			}

		private:
			RegisterItem() = delete;
		};
	}
}
