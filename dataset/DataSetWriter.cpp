#include "DataSetWriter.h"
#include "ElementWriter.h"

#include <emdl/Exception.h>

#include <odil/registry.h>
#include <odil/uid.h>

#include <fstream>
#include <sstream>

namespace emdl
{

	DataSetWriter::DataSetWriter(std::ostream& stream, TransferSyntax transferSyntax, ItemEncoding itemEncoding)
		: BaseWriter(stream, isExplicit(transferSyntax), itemEncoding)
	{
	}

	DataSetWriter::DataSetWriter(std::ostream& stream, bool explicitTS, ItemEncoding itemEncoding)
		: BaseWriter(stream, explicitTS, itemEncoding)
	{
	}

	void DataSetWriter::writeFile(std::ostream& out, const SparseDataSet& dataSet, SparseDataSet metaInformation, ItemEncoding itemEncoding)
	{
		if (dataSet.transferSyntax() == TransferSyntax::Unknown)
			throw Exception("{} Cannot save a data set with no transfer syntax", LOG_POSITION);

		// Build file meta information
		Value::Binaries::value_type::vector_type version = { 0x00, 0x01 };
		metaInformation.set(odil::registry::FileMetaInformationVersion, Value::Binaries({ version }));

		const auto& sopClassUID = dataSet[odil::registry::SOPClassUID];
		if (!sopClassUID)
			throw Exception("{} Missing SOP Class UID", LOG_POSITION);
		if (!sopClassUID->isString())
			throw Exception("{} SOP Class UID is not a string", LOG_POSITION);
		if (sopClassUID->asString().empty())
			throw Exception("{} Empty SOP Class UID", LOG_POSITION);
		metaInformation.set(odil::registry::MediaStorageSOPClassUID, *sopClassUID);

		const auto& sopInstanceUID = dataSet[odil::registry::SOPInstanceUID];
		if (!sopInstanceUID)
			throw Exception("{} Missing SOP Instance UID", LOG_POSITION);
		if (!sopInstanceUID->isString())
			throw Exception("{} SOP Instance UID is not a string", LOG_POSITION);
		if (sopInstanceUID->asString().empty())
			throw Exception("{} Empty SOP Instance UID", LOG_POSITION);
		metaInformation.set(odil::registry::MediaStorageSOPInstanceUID, *sopInstanceUID);

		metaInformation.set(odil::registry::TransferSyntaxUID, { getTransferSyntaxUID(dataSet.transferSyntax()) });
		metaInformation.set(odil::registry::ImplementationClassUID, { odil::implementation_class_uid });
		metaInformation.set(odil::registry::ImplementationVersionName, { odil::implementation_version_name });

		static const auto preamble = std::vector<char>(128, 0);
		out.write(preamble.data(), 128);

		static const auto prefix = std::string("DICM");
		out.write(prefix.data(), 4);

		// Always use explicit VR little endian for the meta information
		DataSetWriter { out, TransferSyntax::ExplicitVRLittleEndian, itemEncoding }.writeDataSet(metaInformation);

		// Data set
		DataSetWriter::writeDataSet(out, dataSet, itemEncoding);
	}

	void DataSetWriter::writeDataSet(std::ostream& out, const SparseDataSet& dataSet, ItemEncoding itemEncoding)
	{
		DataSetWriter { out, dataSet.transferSyntax(), itemEncoding }.writeDataSet(dataSet);
	}
	
	void DataSetWriter::writeDataSet(const SparseDataSet& dataSet)
	{
		bool fastWrite = (isExplicit(dataSet.transferSyntax()) == isExplicitTS());

		for (const auto& group : dataSet.getGroups())
		{
			// For group 0 (command set) and group 2 (file meta information), we have to compute the group length
			// So we write the elements to a temporary buffer first
			if (group.group == 0 || group.group == 2)
			{
				writeTag(odil::Tag(group.group, 0)); // Group length tag

				// Write a dummy value
				const auto lenPos = m_stream.tellp();
				writeElement(Element(Value::Integers({ 0 }), odil::VR::UL));

				// Write the entire group
				const auto startPos = m_stream.tellp();
				for (const auto& tes : group.elements)
					writeElementStruct(dataSet, tes, fastWrite);
				const auto endPos = m_stream.tellp();

				// Update the length
				const auto groupLength = endPos - startPos; // Compute the length of the group
				m_stream.seekp(lenPos); // Go back to the length tag
				writeElement(Element(Value::Integers({ groupLength }), odil::VR::UL)); // Overwrite the length tag
				m_stream.seekp(endPos); // Go back to the end position (could also do a seekp(0, std::ios_bacse::end);)

				TEST_STREAM
			}
			else
			{
				for (const auto& tes : group.elements)
					writeElementStruct(dataSet, tes, fastWrite);
			}
		}
	}

	void DataSetWriter::writeElementStruct(const SparseDataSet& dataSet, const SparseDataSet::TagElementStruct& tes, bool fastWrite) const
	{
		// No raw view for this element
		if (!tes.size)
		{
			writeTag(tes.tag);
			writeElement(dataSet.getElement(tes)); // Use the ElementWriter to write the value
		}
		else
		{
			// Either write the prepared value
			if (!fastWrite || dataSet.isModified(tes))
			{
				// Copy the tag
				m_stream.write(reinterpret_cast<const char*>(&tes.tag), sizeof(odil::Tag));

				// Use the ElementWriter to write the value
				writeElement(dataSet.getElement(tes));
			}
			else // Or directly copy from the raw view
			{
				const auto view = dataSet.getView(tes);
				m_stream.write(reinterpret_cast<char const*>(view.data()), view.size());
			}
		}

		TEST_STREAM
	}

	void DataSetWriter::writeElement(const Element& element) const
	{
		auto writer = ElementWriter(m_stream, isExplicitTS(), itemEncoding());
		writer.writeElement(element);
	}

} // namespace emdl
