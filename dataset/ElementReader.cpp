#include "ElementReader.h"
#include "DataSetReader.h"

#include <emdl/Exception.h>
#include <odil/registry.h>

namespace
{

	emdl::Value::Strings splitString(const std::string& str)
	{
		const size_t nb = 1 + std::count(str.begin(), str.end(), '\\');
		std::vector<std::string> result;
		result.reserve(nb);

		size_t begin = 0, size = str.size();
		unsigned int index = 0;
		while (true)
		{
			const auto end = str.find('\\', begin);
			if (end == std::string::npos)
			{
				result.push_back(str.substr(begin, end));
				break;
			}
			else
			{
				result.push_back(str.substr(begin, end - begin));
				begin = end + 1;
			}
		}

		return result;
	}

}

namespace emdl
{

	ElementReader::ElementReader(const BinaryBufferSPtr& buffer, BinaryView view, TransferSyntax transferSyntax)
		: BaseReader(buffer, view, transferSyntax)
	{
	}

	ElementReader::ElementReader(const BinaryBufferSPtr& buffer, TransferSyntax transferSyntax)
		: BaseReader(buffer, transferSyntax)
	{
	}

	Element ElementReader::readElement(const SparseDataSet& dataSet)
	{
		odil::VR vr = readVR(dataSet); // This reads the tag and the VR

		const auto length = readLength(vr);

		using VR = odil::VR;
		switch (vr)
		{
		case VR::IS:
		case VR::SL:
		case VR::SS:
		case VR::UL:
		case VR::US:
			return { readIntegers(vr, length), vr };

		case VR::DS:
		case VR::FD:
		case VR::FL:
			return { readReals(vr, length), vr };

		case VR::AE:
		case VR::AS:
		case VR::AT:
		case VR::CS:
		case VR::DA:
		case VR::DT:
		case VR::LO:
		case VR::LT:
		case VR::PN:
		case VR::SH:
		case VR::ST:
		case VR::TM:
		case VR::UC:
		case VR::UI:
		case VR::UR:
		case VR::UT:
			return { readStrings(vr, length), vr };

		case VR::SQ:
			return { readDataSets(vr, length), vr };

		case VR::OB:
		case VR::OD:
		case VR::OF:
		case VR::OL:
		case VR::OW:
		case VR::UN:
			return { readBinaries(vr, length), vr };

		case VR::UNKNOWN:
		default:
			throw Exception("{} Unknown VR at position {}", LOG_POSITION, offset());
		}
	}

	odil::VR ElementReader::readVR(const SparseDataSet& dataSet)
	{
		odil::Tag tag = readTag();
		odil::VR vr = odil::VR::UNKNOWN;
		if (isExplicitTS())
			vr = odil::as_vr(readString(2));
		else
			vr = findVR(tag, dataSet);
		return vr;
	}

	Value::Integers ElementReader::readIntegers(odil::VR vr, uint32_t length)
	{
		Value::Integers result;
		using VR = odil::VR;
		if (vr == VR::IS)
		{
			auto const str = readString(length);
			if (!str.empty())
			{
				auto const strList = splitString(str);

				result.resize(strList.size());
				std::transform(strList.begin(), strList.end(), result.begin(), [](const std::string& s) {
					return std::stoll(s);
				});
			}
		}
		else
		{
			uint32_t nb = 0;
			if (vr == VR::SL || vr == VR::UL)
				nb = length / 4;
			else if (vr == VR::AT || vr == VR::SS || vr == VR::US)
				nb = length / 2;
			else
				throw Exception("Cannot read integers from {} at position {}", as_string(vr), offset());

			result.resize(nb);
			if (vr == VR::SL)
			{
				for (unsigned int i = 0; i < nb; ++i)
					result[i] = read<int32_t>();
			}
			else if (vr == VR::SS)
			{
				for (unsigned int i = 0; i < nb; ++i)
					result[i] = read<int16_t>();
			}
			else if (vr == VR::UL)
			{
				for (unsigned int i = 0; i < nb; ++i)
					result[i] = read<uint32_t>();
			}
			else if (vr == VR::AT || vr == VR::US)
			{
				for (unsigned int i = 0; i < nb; ++i)
					result[i] = read<uint16_t>();
			}
		}

		return result;
	}

	Value::Reals ElementReader::readReals(odil::VR vr, uint32_t length)
	{
		Value::Reals result;
		using VR = odil::VR;
		if (vr == VR::DS)
		{
			auto const str = readString(length);
			if (!str.empty())
			{
				auto const strList = splitString(str);
				result.resize(strList.size());
				std::transform(strList.begin(), strList.end(), result.begin(), [](const std::string & s) {
					return static_cast<Value::Real>(std::stold(s));
				});
			}
		}
		else
		{
			uint32_t nb = 0;
			if (vr == VR::FD)
				nb = length / 8;
			else if (vr == VR::FL)
				nb = length / 4;
			else
				throw Exception("Cannot read reals from {} at position {}", as_string(vr), offset());

			result.resize(nb);
			if (vr == VR::FD)
			{
				for (unsigned int i = 0; i < nb; ++i)
					result[i] = read<double>();
			}
			else if (vr == VR::FL)
			{
				for (unsigned int i = 0; i < nb; ++i)
					result[i] = read<float>();
			}
		}

		return result;
	}

	Value::Strings ElementReader::readStrings(odil::VR vr, uint32_t length)
	{
		Value::Strings result;
		using VR = odil::VR;
		if (vr == VR::AT)
		{
			const auto integers = readIntegers(vr, length);

			if (integers.size() % 2 != 0)
				throw Exception("Cannot read AT from odd-sized array at position {}", offset());

			for (size_t i = 0, nb = integers.size(); i < nb; i += 2)
			{
				const auto tag = odil::Tag(static_cast<uint16_t>(integers[i]), static_cast<uint16_t>(integers[i + 1]));
				result.push_back(static_cast<std::string>(tag)); // TODO: use fmt to convert the tag to a string (should be faster)
			}
		}
		else
		{
			const auto str = readString(length);
			if (vr == VR::LT || vr == VR::ST || vr == VR::UT)
				result = { str };
			else
				result = splitString(str);

			// Remove padding
			static const std::string padding = { '\0', ' ' };
			for (auto& item : result)
			{
				const auto last = item.find_last_not_of(padding);
				if (last == item.size()) // No padding
					continue;
				else if (last != std::string::npos)
					item = item.substr(0, last + 1);
				else // Empty string
					item.clear();
			}
		}

		return result;
	}

	Value::DataSets ElementReader::readDataSets(odil::VR vr, uint32_t length)
	{
		Value::DataSets result;
		using VR = odil::VR;
		if (length != 0xffffffff)
		{ // Explicit length sequence
			const auto end = offset() + length;

			while (offset() < end)
			{
				const auto tag = readTag();
				if (tag == odil::registry::Item)
					result.push_back(readItem());
				else
					throw Exception("{} Expected Item, got: {} at position {}", LOG_POSITION, std::string(tag), offset());
			}
		}
		else
		{ // Undefined length sequence
			while (!eof())
			{
				const auto tag = readTag();
				if (tag == odil::registry::Item)
					result.push_back(readItem());
				else if (tag == odil::registry::SequenceDelimitationItem)
				{
					ignore(4);
					break;
				}
				else
					throw Exception("{} Expected SequenceDelimitationItem, got: {} at position {}", LOG_POSITION, std::string(tag), offset());
			}
		}

		return result;
	}

	SparseDataSet ElementReader::readItem()
	{
		const auto itemLength = read<uint32_t>();

		SparseDataSet item;
		if (itemLength != 0xffffffff)
		{ // Explicit length item
			const auto itemView = getView(itemLength);
			auto itemReader = DataSetReader(buffer(), itemView, transferSyntax());
			item = itemReader.readDataSet();
		}
		else
		{ // Undefined length item
			auto remaining = view().size() - offset();
			auto itemView = BinaryView { view().data() + offset(), remaining };
			auto itemReader = DataSetReader(buffer(), itemView, transferSyntax());
			item = itemReader.readDataSet([](const odil::Tag& tag) {
				return tag == odil::registry::ItemDelimitationItem;
			});

			auto const tag = itemReader.readTag();
			if (tag != odil::registry::ItemDelimitationItem)
				throw Exception("{} Unexpected tag: {} at position {}", LOG_POSITION, std::string(tag), offset());
			ignore(itemReader.offset() + 4);
		}

		return item;
	}

	Value::Binaries ElementReader::readBinaries(odil::VR vr, uint32_t length)
	{
		Value::Binaries result;
		using VR = odil::VR;
		if (length == 0)
			return result;
		else if (length == 0xffffffff)
			result = readEncapsulatedPixelData();
		else
			result.emplace_back(getView(length));
		
		return result;
	}

	Value::Binaries ElementReader::readEncapsulatedPixelData()
	{
		Value::Binaries result;

		while (!eof())
		{
			const auto tag = readTag();
			const auto itemLength = read<uint32_t>();

			if(tag == odil::registry::Item)
				result.emplace_back(getView(itemLength));
			else if (tag == odil::registry::SequenceDelimitationItem)
				break;
			else
				throw Exception("{} Expected SequenceDelimitationItem, got: {} at position {}", LOG_POSITION, std::string(tag), offset());
		}

		return result;
	}

}
