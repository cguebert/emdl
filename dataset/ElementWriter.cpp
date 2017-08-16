#include <emdl/dataset/ElementWriter.h>
#include <emdl/dataset/DataSetWriter.h>
#include <emdl/Exception.h>

#include <odil/registry.h>
#include <odil/write_ds.h>

#include <sstream>

#define TEST_STREAM \
	if (!m_stream)  \
		throw Exception("{} Could not write to stream", LOG_POSITION);

#define TEST_STREAM_NONMEMBER \
	if (!stream)              \
		throw emdl::Exception("{} Could not write to stream", LOG_POSITION);

namespace
{
	template <typename T>
	void writeStrings(std::ostream& stream, const T& sequence, char padding)
	{
		if (sequence.empty())
			return;

		const auto start = stream.tellp();
		auto last = --sequence.end();
		for (auto it = sequence.begin(); it != sequence.end(); ++it)
		{
			stream << *it;
			TEST_STREAM_NONMEMBER

			if (it != last)
			{
				stream << "\\";
				TEST_STREAM_NONMEMBER
			}
		}

		const auto end = stream.tellp();
		if ((end - start) % 2 == 1)
		{
			stream.put(padding);
			TEST_STREAM_NONMEMBER
		}
	}

	bool needLargeLength(odil::VR vr)
	{
		using VR = odil::VR;
		return vr == VR::OB || vr == VR::OD || vr == VR::OF || vr == VR::OL || vr == VR::OW || vr == VR::SQ || vr == VR::UC || vr == VR::UR || vr == VR::UT || vr == VR::UN;
	}
}

namespace emdl
{
	ElementWriter::ElementWriter(std::ostream& stream, bool explicitVR, ItemEncoding itemEncoding)
		: BaseWriter(stream, explicitVR, itemEncoding)
	{
	}

	void ElementWriter::writeElement(const Element& element)
	{
		const auto vr = element.vr;
		const auto& value = element.value();

		// If explicit transfer syntax, write the value representation
		if (isExplicitTS())
		{
			m_stream << odil::as_string(vr);
			TEST_STREAM
		}

		// Write a dummy value for the value length
		const auto lenPos = m_stream.tellp();
		bool explicitLargeLength = false;
		if (isExplicitTS())
		{
			explicitLargeLength = needLargeLength(vr);
			if (explicitLargeLength)
			{
				write<uint16_t>(0);
				write<uint32_t>(0);
			}
			else
				write<uint16_t>(0);
		}
		else
			write<uint32_t>(0);

		const auto startPos = m_stream.tellp();
		// Write the value
		if (!value.empty())
		{
			auto visitor = WriterVisitor(m_stream, vr, isExplicitTS(), itemEncoding());
			value.applyVisitor(visitor);
		}
		const auto endPos = m_stream.tellp();
		const auto valueLength = endPos - startPos;

		m_stream.seekp(lenPos); // Go back to the length tag

		// Overwrite the value length
		if (isExplicitTS())
		{
			using VR = odil::VR;
			if (explicitLargeLength)
			{
				write<uint16_t>(0);

				if (vr == VR::SQ && itemEncoding() == ItemEncoding::UndefinedLength)
					write<uint32_t>(0xffffffff);
				else if (is_binary(vr) && element.size() > 1)
					write<uint32_t>(0xffffffff);
				else
					write<uint32_t>(static_cast<uint32_t>(valueLength));
			}
			else
				write<uint16_t>(static_cast<uint32_t>(valueLength));
		}
		else
			write<uint32_t>(static_cast<uint32_t>(valueLength));

		m_stream.seekp(endPos); // Go back to the end position (could also do a seekp(0, std::ios_bacse::end);)

		TEST_STREAM
	}

	/*****************************************************************************/

	ElementWriter::WriterVisitor::WriterVisitor(std::ostream& stream, odil::VR vr, bool explicitVR, BaseWriter::ItemEncoding itemEncoding)
		: BaseWriter(stream, explicitVR, itemEncoding)
		, m_vr(vr)
	{
	}

	void ElementWriter::WriterVisitor::operator()(const Value::Integers& value) const
	{
		using VR = odil::VR;
		switch (m_vr)
		{
		case VR::IS:
			writeStrings(m_stream, value, ' ');
			break;

		case VR::SL:
			for (const auto item : value)
				write<int32_t>(static_cast<int32_t>(item));
			break;

		case VR::SS:
			for (const auto item : value)
				write<int32_t>(static_cast<int32_t>(item));
			break;

		case VR::UL:
			for (const auto item : value)
				write<uint32_t>(static_cast<uint32_t>(item));
			break;

		case VR::AT:
		case VR::US:
			for (const auto item : value)
				write<uint16_t>(static_cast<uint16_t>(item));
			break;

		default:
			throw Exception("Cannot write {} as integers", odil::as_string(m_vr));
		}
	}

	void ElementWriter::WriterVisitor::operator()(const Value::Reals& value) const
	{
		using VR = odil::VR;
		if (m_vr == VR::DS)
		{
			size_t nbWritten = 0;
			for (int i = 0; i < value.size(); ++i)
			{
				if (i)
				{
					m_stream.put('\\');
					nbWritten += 1;
					TEST_STREAM
				}

				const auto& item = value[i];

				if (!std::isfinite(item))
					throw std::exception("DS items must be finite");

				// Each item in the DS is at most 16 bytes, account for NULL at end
				static const unsigned int buffer_size = 16 + 1;
				static char buffer[buffer_size];
				odil::write_ds(item, buffer, buffer_size);
				const auto length = strlen(buffer);

				m_stream.write(buffer, length);
				nbWritten += length;
				TEST_STREAM
			}

			if (nbWritten % 2 == 1)
				m_stream.put(' ');
		}
		else if (m_vr == VR::FD)
		{
			for (const auto& item : value)
				write<double>(item);
		}
		else if (m_vr == VR::FL)
		{
			for (const auto& item : value)
				write<float>(static_cast<float>(item));
		}
		else
			throw Exception("Cannot write {} as reals", odil::as_string(m_vr));
	}

	void ElementWriter::WriterVisitor::operator()(const Value::Strings& value) const
	{
		using VR = odil::VR;
		if (m_vr == VR::AT)
		{
			Value::Integers integers;
			for (const auto& string : value)
			{
				const odil::Tag tag(string);
				integers.push_back(tag.group);
				integers.push_back(tag.element);
			}
			(*this)(integers);
		}
		else
			writeStrings(m_stream, value, (m_vr == VR::UI) ? '\0' : ' ');
	}

	void ElementWriter::WriterVisitor::operator()(const Value::DataSets& value) const
	{
		bool undefinedlength = (itemEncoding() == ItemEncoding::UndefinedLength);

		for (const auto& item : value)
		{
			// Start of item
			writeTag(odil::registry::Item);

			// Item length
			const auto lenPos = m_stream.tellp();
			write<uint32_t>(0xffffffff); // Correct value for undefined length

			// Data set
			const auto startPos = m_stream.tellp();
			DataSetWriter{m_stream, isExplicitTS(), itemEncoding()}.writeDataSet(item);
			const auto endPos = m_stream.tellp();

			if (undefinedlength)
			{ // End of item
				writeTag(odil::registry::ItemDelimitationItem);
				write<uint32_t>(0);
			}
			else
			{ // Overwrite item length
				m_stream.seekp(lenPos); // Go back to the length tag
				const auto valueLength = endPos - startPos;
				write<uint32_t>(static_cast<uint32_t>(valueLength));
				m_stream.seekp(endPos); // Go back to the end position (could also do a seekp(0, std::ios_bacse::end);)
			}

			TEST_STREAM
		}

		// End of item
		if (undefinedlength)
		{
			writeTag(odil::registry::SequenceDelimitationItem);
			write<uint32_t>(0);
		}
	}

	void ElementWriter::WriterVisitor::operator()(const Value::Binaries& value) const
	{
		using VR = odil::VR;
		if (value.empty())
			return;
		else if (value.size() > 1)
			writeEncapsulatedPixelData(value);
		else
		{
			const auto& firstValue = value.front();
			m_stream.write(static_cast<const char*>(firstValue.data()), firstValue.size());

			if (firstValue.size() % 2 == 1)
				m_stream.put('\0');

			TEST_STREAM
		}
	}

	void ElementWriter::WriterVisitor::writeEncapsulatedPixelData(const Value::Binaries& value) const
	{
		for (const auto& fragment : value)
		{
			writeTag(odil::registry::Item);
			const uint32_t length = static_cast<uint32_t>(fragment.size());
			write(length);

			if (length)
			{
				m_stream.write(static_cast<const char*>(fragment.data()), length);
				TEST_STREAM
			}
		}

		writeTag(odil::registry::SequenceDelimitationItem);
		write<uint32_t>(0);
		TEST_STREAM
	}
}
