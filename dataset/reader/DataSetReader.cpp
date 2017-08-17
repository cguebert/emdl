#include <emdl/dataset/reader/DataSetReader.h>
#include <emdl/Exception.h>

#include <odil/registry.h>
#include <fstream>

namespace emdl
{
	BinaryBufferSPtr createBufferFromFile(const std::string& fileName)
	{
		std::ifstream in(fileName, std::ios_base::binary);
		if (in)
		{
			auto buffer = createBufferFromStream(in);
			in.close();
			return buffer;
		}
		else
			throw Exception("Error while opening {}", fileName);
	}

	BinaryBufferSPtr createBufferFromStream(std::istream& stream)
	{
		auto buffer = std::make_shared<BinaryBuffer>();
		stream.seekg(0, std::ios::end);
		buffer->resize((size_t)stream.tellg());
		stream.seekg(0, std::ios::beg);
		stream.read(reinterpret_cast<char*>(&(*buffer)[0]), buffer->size());
		return buffer;
	}

	BinaryBufferSPtr createBufferFromString(const std::string& str)
	{
		auto buffer = std::make_shared<BinaryBuffer>();
		(*buffer).resize(str.size());
		memcpy(buffer->data(), str.data(), str.size());
		return buffer;
	}

	bool isDicomFile(const BinaryBufferSPtr& buffer)
	{
		if (buffer->size() < 132)
			return false;
		auto reader = BaseReader{buffer, TransferSyntax::ExplicitVRLittleEndian};
		reader.ignore(128);
		return reader.readString(4) == "DICM";
	}

	bool isDicomFile(const std::string& filePath)
	{
		std::ifstream in(filePath, std::ios_base::binary);
		if (!in)
			throw Exception("Error while opening {}", filePath);

		in.ignore(128);
		std::string value(4, 0);
		in.read(&value[0], 4);
		return value == "DICM";
	}

	FileSparseDataSets DataSetReader::readFile(const std::string& fileName, HaltConditionFunc func)
	{
		const auto buffer = createBufferFromFile(fileName);

		return readFile(buffer, func);
	}

	FileSparseDataSets DataSetReader::readFile(const BinaryBufferSPtr& buffer, HaltConditionFunc func)
	{
		// File preamble and DICOM prefix
		DataSetReader metaInfoReader(buffer, TransferSyntax::ExplicitVRLittleEndian);
		metaInfoReader.ignore(128);
		const auto prefix = metaInfoReader.readString(4);
		if (prefix != "DICM")
			throw std::exception("Unexpected prefix in DataSetReader::readFile");

		// Read meta information
		const auto metaInfoDataSet = metaInfoReader.readDataSet([](const odil::Tag& tag) {
			return (tag.group != 0x0002);
		});

		const auto transferSyntaxElt = metaInfoDataSet[odil::registry::TransferSyntaxUID];
		if (!transferSyntaxElt)
			throw std::exception("Missing Transfer Syntax UID");
		if (!transferSyntaxElt->isString())
			throw std::exception("Transfer Syntax UID is not a string");
		if (transferSyntaxElt->empty())
			throw std::exception("Empty Transfer Syntax UID");
		const auto& tsStr = transferSyntaxElt->asString()[0];
		auto ts = getTransferSyntax(tsStr);
		if (ts == TransferSyntax::Unknown)
			throw Exception("Transfer syntax not supported: {}", tsStr);

		// Read the remainder of the file
		const auto start = metaInfoReader.offset();
		const auto dataSetView = BinaryView(buffer->data() + start, buffer->size() - start);
		DataSetReader dataSetReader(buffer, dataSetView, ts);
		const auto dataSet = func ? dataSetReader.readDataSet(func) : dataSetReader.readDataSet();

		return {metaInfoDataSet, dataSet};
	}

	/*****************************************************************************/

	DataSetReader::DataSetReader(const BinaryBufferSPtr& buffer, BinaryView view, TransferSyntax transferSyntax)
		: BaseReader(buffer, view, transferSyntax)
	{
	}

	DataSetReader::DataSetReader(const BinaryBufferSPtr& buffer, TransferSyntax transferSyntax)
		: BaseReader(buffer, transferSyntax)
	{
	}

	SparseDataSet DataSetReader::readDataSet()
	{
		SparseDataSet dataSet(buffer(), view(), transferSyntax());
		while (!eof())
		{
			const auto start = offset();
			const auto tag = readTag();
			const auto elt = readElement();
			dataSet.set(tag, BinaryView(view().data() + start, offset() - start));
		}

		dataSet.updateViewSize(offset());
		return dataSet;
	}

	SparseDataSet DataSetReader::readDataSet(HaltConditionFunc haltFunc)
	{
		SparseDataSet dataSet(buffer(), view(), transferSyntax());
		while (!eof())
		{
			auto start = offset();
			const auto tag = readTag();

			if (haltFunc(tag))
			{
				setOffset(start);
				break;
			}

			const auto elt = readElement();
			dataSet.set(tag, BinaryView(view().data() + start, offset() - start));
		}

		dataSet.updateViewSize(offset());
		return dataSet;
	}

	DataSetReader::ElementInfo DataSetReader::readElement()
	{
		ElementInfo elt;
		elt.vr = VR::Unknown;
		elt.length = 0;

		if (isExplicitTS())
			elt.vr = asVr(readString(2));

		elt.length = readLength(elt.vr);

		if (elt.length == 0xFFFFFFFF)
		{ // Parse a sequence
			while (!eof())
			{
				const auto tag = readTag();
				if (tag == odil::registry::Item)
					ignoreItem();
				else if (tag == odil::registry::SequenceDelimitationItem)
				{
					ignore(4);
					break;
				}
				else
					throw Exception("{} Expected SequenceDelimitationItem, got: {} at position {}", LOG_POSITION, std::string(tag), offset());
			}
		}
		else
			ignore(elt.length);

		return elt;
	}

	void DataSetReader::ignoreItem()
	{
		const auto itemLength = read<uint32_t>();
		if (itemLength == 0xFFFFFFFF)
		{
			readDataSet([](const odil::Tag& tag) {
				return tag == odil::registry::ItemDelimitationItem;
			});
			ignore(8);
		}
		else
			ignore(itemLength);
	}

} // namespace emdl
