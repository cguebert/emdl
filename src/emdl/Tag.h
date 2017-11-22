#pragma once

#include <emdl/emdl_api.h>

#include <cstddef>
#include <ostream>
#include <string>

namespace emdl
{
	class EMDL_API Tag
	{
	public:
		Tag() = default;
		Tag(uint16_t group, uint16_t element);
		Tag(const std::string& str); /// Create a tag from the string representation of its numeric value.

		uint16_t group = 0;
		uint16_t element = 0;

		bool operator==(Tag other) const;
		bool operator!=(Tag other) const;
		bool operator<(Tag other) const;
		bool operator>(Tag other) const;
		bool operator<=(Tag other) const;
		bool operator>=(Tag other) const;
	};

	/// Convert a Tag to its string representation.
	EMDL_API std::string asString(Tag tag);

	/// Stream inserter
	EMDL_API std::ostream& operator<<(std::ostream& stream, Tag tag);
}
