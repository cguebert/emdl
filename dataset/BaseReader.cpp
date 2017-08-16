#include <emdl/dataset/BaseReader.h>

#include <fstream>

namespace emdl
{
	BaseReader::BaseReader(const BinaryBufferSPtr& buffer, BinaryView view, TransferSyntax transferSyntax)
		: m_buffer(buffer)
		, m_view(view)
		, m_transferSyntax(transferSyntax)
		, m_explicit(isExplicit(transferSyntax))
	{
	}

	BaseReader::BaseReader(const BinaryBufferSPtr& buffer, TransferSyntax transferSyntax)
		: m_buffer(buffer)
		, m_view(buffer->data(), buffer->size())
		, m_transferSyntax(transferSyntax)
		, m_explicit(isExplicit(transferSyntax))
	{
	}

	const BinaryBufferSPtr& BaseReader::buffer() const
	{
		return m_buffer;
	}

	const BinaryView& BaseReader::view() const
	{
		return m_view;
	}

	TransferSyntax BaseReader::transferSyntax() const
	{
		return m_transferSyntax;
	}

	bool BaseReader::isExplicitTS() const
	{
		return m_explicit;
	}

	bool BaseReader::eof() const
	{
		return m_offset >= m_view.size();
	}

	size_t BaseReader::offset() const
	{
		return m_offset;
	}

	void BaseReader::setOffset(size_t offset)
	{
		m_offset = offset;
	}

	void BaseReader::ignore(size_t offset)
	{
		m_offset += offset;
	}

	odil::Tag BaseReader::readTag()
	{
		odil::Tag tag;
		(*this) >> tag.group;
		(*this) >> tag.element;
		return tag;
	}

	std::string BaseReader::readString(size_t size)
	{
		if (!size)
			return {};
		if (m_view.size() < m_offset + size)
			throw Exception("Could not read from stream in BaseReader::readString, offset: {}, view size: {}, asked: {}",
							m_offset, m_view.size(), size);
		std::string value(size, 0);
		std::memcpy(&value[0], m_view.data() + m_offset, size);
		m_offset += size;
		return value;
	}

	uint32_t BaseReader::readLength(odil::VR vr)
	{
		if (m_explicit)
		{
			switch (vr)
			{
			case odil::VR::OB:
			case odil::VR::OD:
			case odil::VR::OF:
			case odil::VR::OL:
			case odil::VR::OW:
			case odil::VR::SQ:
			case odil::VR::UC:
			case odil::VR::UR:
			case odil::VR::UT:
			case odil::VR::UN:
			{
				ignore(2);
				uint32_t length = 0;
				(*this) >> length;
				return length;
			}
			default:
			{
				uint16_t length;
				(*this) >> length;
				return length;
			}
			}
		}
		else
		{
			uint32_t length = 0;
			(*this) >> length;
			return length;
		}
	}

	BinaryView BaseReader::getView(size_t size)
	{
		if (m_view.size() < m_offset + size)
			throw Exception("Could not read from stream in BaseReader::getView, offset: {}, view size: {}, asked: {}",
							m_offset, m_view.size(), size);

		const auto pos = m_offset;
		m_offset += size;

		return {m_view.data() + pos, size};
	}

} // namespace emdl
