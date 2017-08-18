#include <emdl/Tag.h>
#include <emdl/Exception.h>

#include <iomanip>
#include <sstream>
#include <tuple>

namespace emdl
{
	Tag::Tag(uint16_t group, uint16_t element)
		: group(group)
		, element(element)
	{
	}

	Tag::Tag(const std::string& str)
		: group(static_cast<uint16_t>(std::stoul(str.substr(0, 4), 0, 16)))
		, element(static_cast<uint16_t>(std::stoul(str.substr(4, 8), 0, 16)))
	{
	}

	bool Tag::operator==(Tag other) const
	{
		return ((this->group == other.group) && (this->element == other.element));
	}

	bool Tag::operator!=(Tag other) const
	{
		return !(*this == other);
	}

	bool Tag::operator<(Tag other) const
	{
		return std::tie(group, element) < std::tie(other.group, other.element);
	}

	bool Tag::operator>(Tag other) const
	{
		return std::tie(group, element) > std::tie(other.group, other.element);
	}

	bool Tag::operator<=(Tag other) const
	{
		return !(*this > other);
	}

	bool Tag::operator>=(Tag other) const
	{
		return !(*this < other);
	}

	std::string asString(Tag tag)
	{
		std::ostringstream stream;
		stream << tag;
		return stream.str();
	}

	std::ostream& operator<<(std::ostream& stream, Tag tag)
	{
		std::ostream::char_type const old_fill = stream.fill();
		std::streamsize const old_width = stream.width();
		std::ios::fmtflags const flags = stream.flags();

		stream << std::hex
			   << std::setw(4) << std::setfill('0') << tag.group
			   << std::setw(4) << std::setfill('0') << tag.element;

		stream.setf(flags);
		stream.width(old_width);
		stream.fill(old_fill);

		return stream;
	}
}
