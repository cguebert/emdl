#include <emdl/PrivateDictionary.h>
#include <emdl/dataset/DataSetAccessors.h>

#include <map>

namespace
{
	using PrivateTagKey = std::tuple<std::uint16_t, std::string, std::uint8_t>;
	using PrivateTagsMap = std::map<PrivateTagKey, const emdl::PrivateDictionaryEntry*>;

	const PrivateTagsMap& getPrivateTagsMap()
	{
		static const PrivateTagsMap tagsMap = [] {
			PrivateTagsMap tagsMap;
			const auto& tagsList = emdl::registry::getPrivateDictionary();
			for (const auto& entry : tagsList)
				tagsMap[PrivateTagKey{entry.group, entry.creator, entry.element}] = &entry;
			return tagsMap;
		}();

		return tagsMap;
	}
}

namespace emdl
{
	namespace registry
	{
		const PrivateDictionaryEntry* getPrivateTag(Tag tag, const std::string& creator)
		{
			std::uint8_t element = tag.element & 0xFF;
			PrivateTagKey key{tag.group, creator, element};
			const auto& tagsMap = getPrivateTagsMap();
			const auto it = tagsMap.find(key);
			if (it != tagsMap.end())
				return it->second;

			return nullptr;
		}

		const PrivateDictionaryEntry* getPrivateTag(const DataSet& dataSet, Tag tag)
		{
			const std::uint8_t block = tag.element >> 8;
			emdl::Tag creatorTag{tag.group, block};
			const auto creator = firstString(dataSet, creatorTag);
			if (!creator)
				return nullptr;
			return getPrivateTag(tag, *creator);
		}
	}
}
