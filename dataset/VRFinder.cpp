#include <emdl/dataset/VRFinder.h>
#include <emdl/dataset/DataSet.h>
#include <emdl/dataset/DataSetAccessors.h>
#include <emdl/Exception.h>
#include <emdl/registry.h>
#include <emdl/TagsDictionary.h>

#include <algorithm>

namespace
{
	using ElementVRPair = std::pair<uint16_t, emdl::VR>;
	using ElementVRList = std::vector<ElementVRPair>;
	using GroupPair = std::pair<uint16_t, ElementVRList>;
	using GroupVRList = std::vector<GroupPair>;
	const GroupVRList& publicDictionnary_tags()
	{
		static const GroupVRList groups = [] {
			GroupVRList groups;
			for (const auto& elt : emdl::registry::getPublicDictionary())
			{
				if (elt.first.type() != emdl::TagsDictionaryKey::Type::Tag)
					continue;

				const auto tag = elt.first.tag();
				if (tag == emdl::registry::Item
					|| tag == emdl::registry::ItemDelimitationItem
					|| tag == emdl::registry::SequenceDelimitationItem)
					continue;

				emdl::VR vr = emdl::VR::Unknown;
				try
				{
					vr = emdl::asVR(std::string{elt.second.vr}.substr(0, 2));
				} // HACK: only take the first VR when there are two or more (better to have something than not)
				catch (...)
				{
					continue;
				}

				// Find the group
				auto grIt = std::lower_bound(groups.begin(), groups.end(), tag, [](const GroupPair& lhs, const emdl::Tag rhs) {
					return lhs.first < rhs.group;
				});

				// Add the group if it does not exist
				if (grIt == groups.end() || grIt->first != tag.group)
				{
					GroupPair g;
					g.first = tag.group;
					grIt = groups.insert(grIt, g);
				}

				// Find where to insert the element
				auto& elements = grIt->second;
				auto elIt = std::lower_bound(elements.begin(), elements.end(), tag, [](const ElementVRPair& lhs, const emdl::Tag rhs) {
					return lhs.first < rhs.element;
				});

				ElementVRPair e;
				e.first = tag.element;
				e.second = vr;
				elements.insert(elIt, e);
			}
			return groups;
		}();

		return groups;
	}

	using StringVRPair = std::pair<std::string, emdl::VR>;
	using StringVRList = std::vector<StringVRPair>;
	const StringVRList& publicDictionnary_strings()
	{
		static const StringVRList strings = [] {
			StringVRList strings;
			for (const auto& elt : emdl::registry::getPublicDictionary())
			{
				if (elt.first.type() == emdl::TagsDictionaryKey::Type::String)
				{
					emdl::VR vr = emdl::VR::Unknown;
					try
					{
						vr = emdl::asVR(elt.second.vr);
					}
					catch (...)
					{
					}
					strings.emplace_back(elt.first.string(), vr);
				}
			}

			std::sort(strings.begin(), strings.end(), [](const StringVRPair& lhs, const StringVRPair& rhs) {
				return lhs.first < rhs.first;
			});
			return strings;
		}();

		return strings;
	}

	emdl::VR publicDictionary(const emdl::Tag tag)
	{
		const auto& groups = publicDictionnary_tags();
		auto grIt = std::find_if(groups.begin(), groups.end(), [&tag](const GroupPair& g) {
			return g.first == tag.group;
		});
		/*	auto grIt = std::lower_bound(groups.begin(), groups.end(), tag, [](const GroupPair& lhs, const Tag& rhs) {
			return lhs.first < rhs.group;
		});
		*/
		if (grIt != groups.end() && grIt->first == tag.group)
		{
			auto& elements = grIt->second;
			/*	auto elIt = std::find_if(elements.begin(), elements.end(), [&tag](const ElementVRPair& e) {
				return e.first == tag.element;
			});*/
			auto elIt = std::lower_bound(elements.begin(), elements.end(), tag, [](const ElementVRPair& lhs, const emdl::Tag rhs) {
				return lhs.first < rhs.element;
			});

			if (elIt != elements.end() && elIt->first == tag.element)
				return elIt->second;
		}

		auto tagStr = asString(tag);
		tagStr[2] = tagStr[3] = 'x';
		const auto& strings = publicDictionnary_strings();
		const auto it2 = std::lower_bound(strings.begin(), strings.end(), tagStr, [](const StringVRPair& lhs, const std::string& rhs) {
			return lhs.first < rhs;
		});

		if (it2 != strings.end() && it2->first == tagStr)
			return it2->second;

		return emdl::VR::UN;
	}

	emdl::VR groupLength(const emdl::Tag tag, const emdl::DataSet&, emdl::TransferSyntax)
	{
		if (tag.element == 0)
			return emdl::VR::UL;

		return emdl::VR::Unknown;
	}

	emdl::VR privateTag(const emdl::Tag tag, const emdl::DataSet&, emdl::TransferSyntax)
	{
		if (tag.group % 2 == 1)
			return emdl::VR::UN;

		return emdl::VR::Unknown;
	}

	emdl::VR implicitVRLittleEndian(const emdl::Tag tag, const emdl::DataSet& dataSet, emdl::TransferSyntax transferSyntax)
	{
		if (transferSyntax != emdl::TransferSyntax::ImplicitVRLittleEndian)
			return emdl::VR::Unknown;

		namespace reg = emdl::registry;
		if (tag == reg::PixelData)
			return emdl::VR::OW;
		else if ((tag.group >> 8) == 0x60 && tag.element == 0x3000)
			return emdl::VR::OW;
		else if (tag == reg::WaveformData)
			return emdl::VR::OW;
		else if (tag == reg::RedPaletteColorLookupTableData
				 || tag == reg::GreenPaletteColorLookupTableData
				 || tag == reg::BluePaletteColorLookupTableData
				 || tag == reg::AlphaPaletteColorLookupTableData)
			return emdl::VR::OW;
		else if (tag == reg::SegmentedRedPaletteColorLookupTableData
				 || tag == reg::SegmentedGreenPaletteColorLookupTableData
				 || tag == reg::SegmentedBluePaletteColorLookupTableData)
			return emdl::VR::OW;
		else if (tag == reg::BlendingLookupTableData)
			return emdl::VR::OW;
		else if (tag == reg::VertexPointIndexList
				 || tag == reg::EdgePointIndexList
				 || tag == reg::TrianglePointIndexList
				 || tag == reg::PrimitivePointIndexList)
			return emdl::VR::OW;
		else if (tag == reg::SmallestImagePixelValue
				 || tag == reg::LargestImagePixelValue
				 || tag == reg::SmallestPixelValueInSeries
				 || tag == reg::LargestPixelValueInSeries
				 || tag == reg::PixelPaddingValue)
		{
			if (!dataSet.has(reg::PixelRepresentation))
				throw emdl::Exception("Cannot find VR without PixelRepresentation");
			const auto pixelRepresentation = asInt(dataSet, reg::PixelRepresentation, 0);
			return pixelRepresentation ? emdl::VR::SS : emdl::VR::US;
		}
		else
			return emdl::VR::Unknown;
	}

	emdl::VR explicitVRLittleEndian(const emdl::Tag tag, const emdl::DataSet& dataSet, emdl::TransferSyntax transferSyntax)
	{
		if (transferSyntax != emdl::TransferSyntax::ExplicitVRLittleEndian)
			return emdl::VR::Unknown;

		namespace reg = emdl::registry;
		if (tag == reg::PixelData)
		{
			if (!dataSet.has(reg::BitsAllocated))
				throw emdl::Exception("Cannot find VR without BitsAllocated");
			auto const& bits_allocated = asInt(dataSet, reg::BitsAllocated, 0);
			return (bits_allocated <= 8) ? emdl::VR::OB : emdl::VR::OW;
		}
		else if ((tag.group >> 8) == 0x60 && tag.element == 0x3000)
			return emdl::VR::OW;
		else if (tag == reg::WaveformData)
			return emdl::VR::OW;
		else if (tag == reg::RedPaletteColorLookupTableData
				 || tag == reg::GreenPaletteColorLookupTableData
				 || tag == reg::BluePaletteColorLookupTableData
				 || tag == reg::AlphaPaletteColorLookupTableData)
			return emdl::VR::OW;
		else if (tag == reg::SegmentedRedPaletteColorLookupTableData
				 || tag == reg::SegmentedGreenPaletteColorLookupTableData
				 || tag == reg::SegmentedBluePaletteColorLookupTableData)
			return emdl::VR::OW;
		else if (tag == reg::BlendingLookupTableData)
			return emdl::VR::OW;
		else if (tag == reg::VertexPointIndexList
				 || tag == reg::EdgePointIndexList
				 || tag == reg::TrianglePointIndexList
				 || tag == reg::PrimitivePointIndexList)
			return emdl::VR::OW;
		else if (tag == reg::SmallestImagePixelValue
				 || tag == reg::LargestImagePixelValue
				 || tag == reg::SmallestPixelValueInSeries
				 || tag == reg::LargestPixelValueInSeries
				 || tag == reg::PixelPaddingValue)
		{
			if (!dataSet.has(reg::PixelRepresentation))
				throw emdl::Exception("Cannot find VR without PixelRepresentation");
			const auto pixelRepresentation = asInt(dataSet, reg::PixelRepresentation, 0);
			return pixelRepresentation ? emdl::VR::SS : emdl::VR::US;
		}
		else
			return emdl::VR::Unknown;
	}
}

namespace emdl
{
	VR findVR(const Tag tag)
	{
		return publicDictionary(tag);
	}

	VR findVR(const Tag tag, const DataSet& dataSet)
	{
		TransferSyntax transferSyntax = dataSet.transferSyntax();
		for (const auto& func : {groupLength, privateTag, implicitVRLittleEndian, explicitVRLittleEndian})
		{
			auto vr = func(tag, dataSet, transferSyntax);
			if (vr != emdl::VR::Unknown)
				return vr;
		}

		return publicDictionary(tag);
	}

} // namespace emdl
