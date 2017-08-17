#include <emdl/dataset/VRFinder.h>
#include <emdl/dataset/SparseDataSet.h>
#include <emdl/dataset/DataSetAccessors.h>

#include <odil/ElementsDictionary.h>
#include <odil/registry.h>

#include <algorithm>

namespace
{
	using ElementVRPair = std::pair<uint16_t, emdl::VR>;
	using ElementVRList = std::vector<ElementVRPair>;
	using GroupPair = std::pair<uint16_t, ElementVRList>;
	using GroupVRList = std::vector<GroupPair>;
	const GroupVRList& publicDictionnary_tags()
	{
		static GroupVRList groups;
		if (groups.empty())
		{
			for (const auto& elt : odil::registry::public_dictionary)
			{
				if (elt.first.get_type() != odil::ElementsDictionaryKey::Type::Tag)
					continue;

				const auto& tag = elt.first.get_tag();
				if (tag == odil::registry::Item
					|| tag == odil::registry::ItemDelimitationItem
					|| tag == odil::registry::SequenceDelimitationItem)
					continue;

				emdl::VR vr = emdl::VR::Unknown;
				try
				{
					vr = emdl::asVr(elt.second.vr.substr(0, 2));
				} // HACK: only take the first VR when there are two or more (better to have something than not)
				catch (...)
				{
					continue;
				}

				// Find the group
				auto grIt = std::lower_bound(groups.begin(), groups.end(), tag, [](const GroupPair& lhs, const odil::Tag& rhs) {
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
				auto elIt = std::lower_bound(elements.begin(), elements.end(), tag, [](const ElementVRPair& lhs, const odil::Tag& rhs) {
					return lhs.first < rhs.element;
				});

				ElementVRPair e;
				e.first = tag.element;
				e.second = vr;
				elements.insert(elIt, e);
			}
		}

		return groups;
	}

	using StringVRPair = std::pair<std::string, emdl::VR>;
	using StringVRList = std::vector<StringVRPair>;
	const StringVRList& publicDictionnary_strings()
	{
		static StringVRList strings;
		if (strings.empty())
		{
			for (const auto& elt : odil::registry::public_dictionary)
			{
				if (elt.first.get_type() == odil::ElementsDictionaryKey::Type::String)
				{
					emdl::VR vr = emdl::VR::Unknown;
					try
					{
						vr = emdl::asVr(elt.second.vr);
					}
					catch (...)
					{
					}
					strings.emplace_back(elt.first.get_string(), vr);
				}
			}

			std::sort(strings.begin(), strings.end(), [](const StringVRPair& lhs, const StringVRPair& rhs) {
				return lhs.first < rhs.first;
			});
		}

		return strings;
	}

	emdl::VR publicDictionary(const odil::Tag& tag)
	{
		const auto& groups = publicDictionnary_tags();
		auto grIt = std::find_if(groups.begin(), groups.end(), [&tag](const GroupPair& g) {
			return g.first == tag.group;
		});
		/*	auto grIt = std::lower_bound(groups.begin(), groups.end(), tag, [](const GroupPair& lhs, const odil::Tag& rhs) {
			return lhs.first < rhs.group;
		});
		*/
		if (grIt != groups.end() && grIt->first == tag.group)
		{
			auto& elements = grIt->second;
			/*	auto elIt = std::find_if(elements.begin(), elements.end(), [&tag](const ElementVRPair& e) {
				return e.first == tag.element;
			});*/
			auto elIt = std::lower_bound(elements.begin(), elements.end(), tag, [](const ElementVRPair& lhs, const odil::Tag& rhs) {
				return lhs.first < rhs.element;
			});

			if (elIt != elements.end() && elIt->first == tag.element)
				return elIt->second;
		}

		std::string tagStr(tag);
		tagStr[2] = tagStr[3] = 'x';
		const auto& strings = publicDictionnary_strings();
		const auto it2 = std::lower_bound(strings.begin(), strings.end(), tagStr, [](const StringVRPair& lhs, const std::string& rhs) {
			return lhs.first < rhs;
		});

		if (it2 != strings.end() && it2->first == tagStr)
			return it2->second;

		return emdl::VR::UN;
	}

	emdl::VR groupLength(const odil::Tag& tag, const emdl::SparseDataSet&, emdl::TransferSyntax)
	{
		if (tag.element == 0)
			return emdl::VR::UL;

		return emdl::VR::Unknown;
	}

	emdl::VR privateTag(const odil::Tag& tag, const emdl::SparseDataSet&, emdl::TransferSyntax)
	{
		if (tag.group % 2 == 1)
			return emdl::VR::UN;

		return emdl::VR::Unknown;
	}

	emdl::VR implicitVRLittleEndian(const odil::Tag& tag, const emdl::SparseDataSet& dataSet, emdl::TransferSyntax transferSyntax)
	{
		if (transferSyntax != emdl::TransferSyntax::ImplicitVRLittleEndian)
			return emdl::VR::Unknown;

		if (tag == odil::registry::PixelData)
			return emdl::VR::OW;
		else if ((tag.group >> 8) == 0x60 && tag.element == 0x3000)
			return emdl::VR::OW;
		else if (tag == odil::registry::WaveformData)
			return emdl::VR::OW;
		else if (tag == odil::registry::RedPaletteColorLookupTableData
				 || tag == odil::registry::GreenPaletteColorLookupTableData
				 || tag == odil::registry::BluePaletteColorLookupTableData
				 || tag == odil::registry::AlphaPaletteColorLookupTableData)
			return emdl::VR::OW;
		else if (tag == odil::registry::SegmentedRedPaletteColorLookupTableData
				 || tag == odil::registry::SegmentedGreenPaletteColorLookupTableData
				 || tag == odil::registry::SegmentedBluePaletteColorLookupTableData)
			return emdl::VR::OW;
		else if (tag == odil::registry::BlendingLookupTableData)
			return emdl::VR::OW;
		else if (tag == odil::registry::VertexPointIndexList
				 || tag == odil::registry::EdgePointIndexList
				 || tag == odil::registry::TrianglePointIndexList
				 || tag == odil::registry::PrimitivePointIndexList)
			return emdl::VR::OW;
		else if (tag == odil::registry::SmallestImagePixelValue
				 || tag == odil::registry::LargestImagePixelValue
				 || tag == odil::registry::SmallestPixelValueInSeries
				 || tag == odil::registry::LargestPixelValueInSeries
				 || tag == odil::registry::PixelPaddingValue)
		{
			if (!dataSet.has(odil::registry::PixelRepresentation))
				throw std::exception("Cannot find VR without PixelRepresentation");
			const auto pixelRepresentation = asInt(dataSet, odil::registry::PixelRepresentation, 0);
			return pixelRepresentation ? emdl::VR::SS : emdl::VR::US;
		}
		else
			return emdl::VR::Unknown;
	}

	emdl::VR explicitVRLittleEndian(const odil::Tag& tag, const emdl::SparseDataSet& dataSet, emdl::TransferSyntax transferSyntax)
	{
		if (transferSyntax != emdl::TransferSyntax::ExplicitVRLittleEndian)
			return emdl::VR::Unknown;

		if (tag == odil::registry::PixelData)
		{
			if (!dataSet.has(odil::registry::BitsAllocated))
				throw std::exception("Cannot find VR without BitsAllocated");
			auto const& bits_allocated = asInt(dataSet, odil::registry::BitsAllocated, 0);
			return (bits_allocated <= 8) ? emdl::VR::OB : emdl::VR::OW;
		}
		else if ((tag.group >> 8) == 0x60 && tag.element == 0x3000)
			return emdl::VR::OW;
		else if (tag == odil::registry::WaveformData)
			return emdl::VR::OW;
		else if (tag == odil::registry::RedPaletteColorLookupTableData
				 || tag == odil::registry::GreenPaletteColorLookupTableData
				 || tag == odil::registry::BluePaletteColorLookupTableData
				 || tag == odil::registry::AlphaPaletteColorLookupTableData)
			return emdl::VR::OW;
		else if (tag == odil::registry::SegmentedRedPaletteColorLookupTableData
				 || tag == odil::registry::SegmentedGreenPaletteColorLookupTableData
				 || tag == odil::registry::SegmentedBluePaletteColorLookupTableData)
			return emdl::VR::OW;
		else if (tag == odil::registry::BlendingLookupTableData)
			return emdl::VR::OW;
		else if (tag == odil::registry::VertexPointIndexList
				 || tag == odil::registry::EdgePointIndexList
				 || tag == odil::registry::TrianglePointIndexList
				 || tag == odil::registry::PrimitivePointIndexList)
			return emdl::VR::OW;
		else if (tag == odil::registry::SmallestImagePixelValue
				 || tag == odil::registry::LargestImagePixelValue
				 || tag == odil::registry::SmallestPixelValueInSeries
				 || tag == odil::registry::LargestPixelValueInSeries
				 || tag == odil::registry::PixelPaddingValue)
		{
			if (!dataSet.has(odil::registry::PixelRepresentation))
				throw std::exception("Cannot find VR without PixelRepresentation");
			const auto pixelRepresentation = asInt(dataSet, odil::registry::PixelRepresentation, 0);
			return pixelRepresentation ? emdl::VR::SS : emdl::VR::US;
		}
		else
			return emdl::VR::Unknown;
	}
}

namespace emdl
{
	VR findVR(const odil::Tag& tag)
	{
		return publicDictionary(tag);
	}

	VR findVR(const odil::Tag& tag, const SparseDataSet& dataSet)
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
