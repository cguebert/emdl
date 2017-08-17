#pragma once

#include <emdl/Tag.h>

#include <map>

namespace emdl
{
	/// Key of a dictionary of DICOM elements.
	class EMDL_API TagsDictionaryKey
	{
	public:
		/// Type of the key.
		enum class Type
		{
			Tag,
			String,
			None
		};

		TagsDictionaryKey() = default;
		TagsDictionaryKey(const Tag tag);
		TagsDictionaryKey(const char* str);
		TagsDictionaryKey(const std::string& str);

		Type type() const;

		Tag tag() const; /// Return the tag value or raise an exception if type is not Tag.
		const std::string& string() const; /// Return the string value or raise an exception if type is not String.

		bool operator<(TagsDictionaryKey const& other) const;
		bool operator==(TagsDictionaryKey const& other) const;

	private:
		Type m_type = Type::None;
		const Tag m_tag;
		const std::string m_string;
	};

	/// Entry in a dictionary of DICOM tags
	struct EMDL_API TagsDictionaryEntry
	{
		TagsDictionaryEntry(const char* name, const char* keyword, const char* vr, const char* vm);

		const char* name; /// Full name.
		const char* keyword; /// Brief name.
		const char* vr; /// Type.
		const char* vm; /// Multiplicity.
	};

	using TagsDictionary = std::map<TagsDictionaryKey, TagsDictionaryEntry>;

	EMDL_API TagsDictionary::const_iterator find(const TagsDictionary& dictionary, Tag tag);

	namespace registry
	{
		EMDL_API const TagsDictionary& getPublicDictionary();
	}
}
