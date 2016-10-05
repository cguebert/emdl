#pragma once

#include <emdl/Exception.h>
#include <emdl/TransferSyntaxes.h>

#include <ostream>

#define TEST_STREAM if(!m_stream) \
		throw Exception("{} Could not write to stream", LOG_POSITION);

namespace odil
{
	class Tag;
}

namespace emdl
{

	class BaseWriter
	{
	public:
		enum class ItemEncoding : char { ExplicitLength, UndefinedLength };

		BaseWriter(std::ostream& stream, bool explicitTS, ItemEncoding itemEncoding = ItemEncoding::ExplicitLength);

		bool isExplicitTS() const;
		ItemEncoding itemEncoding() const;

		void writeTag(const odil::Tag& tag) const;

		template <class T>
		void write(const T& value) const
		{
			m_stream.write(reinterpret_cast<const char*>(&value), sizeof(value));
			if(!m_stream)
				throw std::exception("BaseWriter: could not write a value to stream");
		}

	protected:
		std::ostream& m_stream;
		const bool m_explicit;
		const ItemEncoding m_itemEncoding;
	};

} // namespace emdl
