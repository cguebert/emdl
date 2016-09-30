#include "BaseWriter.h"

#include <odil/DataSet.h>
#include <odil/registry.h>

#include <fstream>

namespace emds
{

	BaseWriter::BaseWriter(std::ostream& stream, bool explicitTS, ItemEncoding itemEncoding)
		: m_stream(stream)
		, m_explicit(explicitTS)
		, m_itemEncoding(itemEncoding)
	{
	}

	bool BaseWriter::isExplicitTS() const
	{
		return m_explicit;
	}

	BaseWriter::ItemEncoding BaseWriter::itemEncoding() const
	{
		return m_itemEncoding;
	}

	void BaseWriter::writeTag(const odil::Tag& tag) const
	{
		write(tag.group);
		write(tag.element);
	}

} // namespace emds
