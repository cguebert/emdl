#pragma once

#include <emdl/Exception.h>
#include <emdl/TransferSyntaxes.h>
#include <emdl/Tag.h>

#include <ostream>

namespace emdl
{
	class EMDL_API BaseWriter
	{
	public:
		enum class ItemEncoding : char
		{
			ExplicitLength,
			UndefinedLength
		};

		BaseWriter(std::ostream& stream, bool explicitTS, ItemEncoding itemEncoding = ItemEncoding::ExplicitLength);

		bool isExplicitTS() const;
		ItemEncoding itemEncoding() const;

		void writeTag(const Tag tag) const;

		template <class T>
		void write(const T& value) const
		{
			m_stream.write(reinterpret_cast<const char*>(&value), sizeof(value));
			if (!m_stream)
				throw Exception("BaseWriter: could not write a value to stream");
		}

	protected:
		std::ostream& m_stream;
		const bool m_explicit;
		const ItemEncoding m_itemEncoding;
	};

} // namespace emdl
