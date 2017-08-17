#include <emdl/TagsDictionary.h>
#include <emdl/Exception.h>

namespace emdl
{
	TagsDictionaryKey::TagsDictionaryKey(Tag tag)
		: m_type(Type::Tag)
		, m_tag(tag)
	{
	}

	TagsDictionaryKey::TagsDictionaryKey(const char* str)
		: m_type(Type::String)
		, m_string(str)
	{
	}

	TagsDictionaryKey::TagsDictionaryKey(const std::string& str)
		: m_type(Type::String)
		, m_string(str)
	{
	}

	TagsDictionaryKey::Type TagsDictionaryKey::type() const
	{
		return m_type;
	}

	Tag TagsDictionaryKey::tag() const
	{
		if (m_type != Type::Tag)
			throw Exception("Invalid type");
		return m_tag;
	}

	const std::string& TagsDictionaryKey::string() const
	{
		if (m_type != Type::String)
			throw Exception("Invalid type");
		return m_string;
	}

	bool TagsDictionaryKey::operator<(const TagsDictionaryKey& other) const
	{
		if (m_type == other.m_type)
		{
			if (m_type == Type::Tag)
				return m_tag < other.m_tag;
			else if (m_type == Type::String)
				return m_string < other.m_string;
			else
				throw Exception("Invalid type");
		}
		else
			return static_cast<int>(m_type) < static_cast<int>(other.m_type);
	}

	bool TagsDictionaryKey::operator==(const TagsDictionaryKey& other) const
	{
		if (m_type == other.m_type)
		{
			if (m_type == Type::Tag)
				return (m_tag == other.m_tag);
			else if (m_type == Type::String)
				return (m_string == other.m_string);
			else
				throw Exception("Invalid type");
		}
		else
			return false;
	}

	/*****************************************************************************/

	TagsDictionaryEntry::TagsDictionaryEntry(const char* name, const char* keyword, const char* vr, const char* vm)
		: name(name)
		, keyword(keyword)
		, vr(vr)
		, vm(vm)
	{
	}

	TagsDictionary::const_iterator find(const TagsDictionary& dictionary, Tag tag)
	{
		auto it = dictionary.find(tag);
		if (it == dictionary.end())
		{
			std::string tagString(tag);
			tagString[2] = tagString[3] = 'x';

			it = dictionary.find(tagString);
		}

		return it;
	}
}
