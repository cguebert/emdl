#include <emdl/dataset/writer/DataSetWriter.h>
#include <emdl/dataset/writer/ElementWriter.h>
#include <emdl/registry.h>
#include <emdl/association/AssociationParameters.h>

#include <fstream>
#include <sstream>

#define TEST_STREAM \
	if (!m_stream)  \
		throw Exception("{} Could not write to stream", LOG_POSITION);

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

	void DataSetWriter::writeFile(std::ostream& out, const DataSet& dataSet, DataSet metaInformation, ItemEncoding itemEncoding)
	{
		if (dataSet.transferSyntax() == TransferSyntax::Unknown)
			throw Exception("{} Cannot save a data set with no transfer syntax", LOG_POSITION);

		// Build file meta information
		Value::Binaries::value_type::vector_type version = {0x00, 0x01};
		metaInformation.set(registry::FileMetaInformationVersion, Value::Binaries({version}));

		const auto& sopClassUID = dataSet[registry::SOPClassUID];
		if (!sopClassUID)
			throw Exception("{} Missing SOP Class UID", LOG_POSITION);
		if (!sopClassUID->isString())
			throw Exception("{} SOP Class UID is not a string", LOG_POSITION);
		if (sopClassUID->asString().empty())
			throw Exception("{} Empty SOP Class UID", LOG_POSITION);
		metaInformation.set(registry::MediaStorageSOPClassUID, *sopClassUID);

		const auto& sopInstanceUID = dataSet[registry::SOPInstanceUID];
		if (!sopInstanceUID)
			throw Exception("{} Missing SOP Instance UID", LOG_POSITION);
		if (!sopInstanceUID->isString())
			throw Exception("{} SOP Instance UID is not a string", LOG_POSITION);
		if (sopInstanceUID->asString().empty())
			throw Exception("{} Empty SOP Instance UID", LOG_POSITION);
		metaInformation.set(registry::MediaStorageSOPInstanceUID, *sopInstanceUID);

		metaInformation.set(registry::TransferSyntaxUID, {getTransferSyntaxUID(dataSet.transferSyntax())});
		metaInformation.set(registry::ImplementationClassUID, {emdl::defaultImplementationClassUid});
		metaInformation.set(registry::ImplementationVersionName, {emdl::defaultImplementationVersionName});

		static const auto preamble = std::vector<char>(128, 0);
		out.write(preamble.data(), 128);

		static const auto prefix = std::string("DICM");
		out.write(prefix.data(), 4);

		// Always use explicit VR little endian for the meta information
		DataSetWriter{out, TransferSyntax::ExplicitVRLittleEndian, itemEncoding}.writeDataSet(metaInformation);

		// Data set
		DataSetWriter::writeDataSet(out, dataSet, itemEncoding);
	}

	void DataSetWriter::writeDataSet(std::ostream& out, const DataSet& dataSet, ItemEncoding itemEncoding)
	{
		DataSetWriter{out, dataSet.transferSyntax(), itemEncoding}.writeDataSet(dataSet);
	}

	void DataSetWriter::writeDataSet(const DataSet& dataSet)
	{
		bool fastWrite = (isExplicit(dataSet.transferSyntax()) == isExplicitTS());

		for (const auto& group : dataSet.getGroups())
		{
			// For group 0 (command set) and group 2 (file meta information), we have to compute the group length
			if (group.group == 0 || group.group == 2)
			{
				writeTag(Tag(group.group, 0)); // Group length tag

				// Write a dummy value
				const auto lenPos = m_stream.tellp();
				writeElement(Element(Value::Integers({0}), VR::UL));

				// Write the entire group
				const auto startPos = m_stream.tellp();
				for (const auto& tes : group.elements)
				{
					if (tes.tag.element != 0) //do not write group length twice if already present in dataset
						writeElementStruct(dataSet, tes, fastWrite);
				}
				const auto endPos = m_stream.tellp();

				// Update the length
				const auto groupLength = endPos - startPos; // Compute the length of the group
				m_stream.seekp(lenPos); // Go back to the length tag
				writeElement(Element(Value::Integers({groupLength}), VR::UL)); // Overwrite the length tag
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

	void DataSetWriter::writeElementStruct(const DataSet& dataSet, const DataSet::TagElementStruct& tes, bool fastWrite) const
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
				m_stream.write(reinterpret_cast<const char*>(&tes.tag), sizeof(Tag));

				// Use the ElementWriter to write the value
				writeElement(dataSet.getElement(tes));
			}
			else // Or directly copy from the raw view
			{
				const auto view = dataSet.getView(tes);
				m_stream.write(reinterpret_cast<const char*>(view.data()), view.size());
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
