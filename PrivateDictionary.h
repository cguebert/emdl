#pragma once

#include <emdl/Tag.h>

#include <vector>

namespace emdl
{
	/// Entry in a dictionary of private DICOM tags
	struct EMDL_API PrivateDictionaryEntry
	{
		PrivateDictionaryEntry(std::uint16_t group,
							   const char* creator,
							   std::uint8_t element,
							   const char* keyword,
							   const char* vr,
							   const char* vm,
							   const char* code)
			: group(group)
			, creator(creator)
			, element(element)
			, keyword(keyword)
			, vr(vr)
			, vm(vm)
			, code(code)
		{
		}

		std::uint16_t group = 0; /// Group containing this private tag.
		const char* creator; /// Creator of this private tag.
		std::uint8_t element = 0; /// Last 8 bits of the element part of the tag.
		const char* keyword; /// Brief name of the tag.
		const char* vr; /// Type.
		const char* vm; /// Multiplicity.
		const char* code; /// Code for the de-identification action.
	};

	using PrivateTags = std::vector<PrivateDictionaryEntry>;

	class DataSet;

	namespace registry
	{
		EMDL_API const PrivateTags& getPrivateDictionary();
		EMDL_API const PrivateDictionaryEntry* getPrivateTag(Tag tag, const std::string& creator);
		EMDL_API const PrivateDictionaryEntry* getPrivateTag(const DataSet& dataSet, Tag tag); // Will look for the creator and then return the corresponding entry or null
	}
}
