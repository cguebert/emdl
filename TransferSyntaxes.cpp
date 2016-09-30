#include "TransferSyntaxes.h"

#include <boost/container/flat_map.hpp>

#include <algorithm>
#include <vector>

namespace
{

	using TransferSyntaxPair = std::pair<emds::TransferSyntax, std::string>;
	using TransferSyntaxPairs = std::vector<TransferSyntaxPair>;
	const TransferSyntaxPairs& getTransferSyntaxPairs()
	{
		using TS = emds::TransferSyntax;
		static TransferSyntaxPairs transferSyntaxes = {
			{ TS::Unknown,                              "Not supported" },
			{ TS::ImplicitVRLittleEndian,               "1.2.840.10008.1.2" },
			{ TS::ExplicitVRLittleEndian,               "1.2.840.10008.1.2.1"},
			{ TS::DeflatedExplicitVRLittleEndian,       "1.2.840.10008.1.2.1.99"},
			{ TS::JPEGBaselineProcess1,					"1.2.840.10008.1.2.4.50"},
			{ TS::JPEGExtendedProcess2_4,				"1.2.840.10008.1.2.4.51"},
			{ TS::JPEGExtendedProcess3_5,				"1.2.840.10008.1.2.4.52"},
			{ TS::JPEGSpectralSelectionProcess6_8,		"1.2.840.10008.1.2.4.53"},
			{ TS::JPEGFullProgressionProcess10_12,		"1.2.840.10008.1.2.4.55"},
			{ TS::JPEGLosslessProcess14,				"1.2.840.10008.1.2.4.57"},
			{ TS::JPEGLosslessProcess14SV1,				"1.2.840.10008.1.2.4.70"},
			{ TS::JPEGLSLossless,                       "1.2.840.10008.1.2.4.80"},
			{ TS::JPEGLSLossy,                          "1.2.840.10008.1.2.4.81"},
			{ TS::JPEG2000Lossless,						"1.2.840.10008.1.2.4.90"},
			{ TS::JPEG2000,                             "1.2.840.10008.1.2.4.91"},
			{ TS::JPEG2000Part2Lossless,				"1.2.840.10008.1.2.4.92"},
			{ TS::JPEG2000Part2,                        "1.2.840.10008.1.2.4.93"},
			{ TS::JPIPReferenced,                       "1.2.840.10008.1.2.4.94"},
			{ TS::JPIPReferencedDeflate,                "1.2.840.10008.1.2.4.95"},
			{ TS::MPEG2MainProfile,						"1.2.840.10008.1.2.4.100"},
			{ TS::MPEG2MainProfileHighLevel,			"1.2.840.10008.1.2.4.101"},
			{ TS::MPEG4HighProfileLevel4_1,             "1.2.840.10008.1.2.4.102"},
			{ TS::MPEG4BDcompatibleHighProfileLevel4_1, "1.2.840.10008.1.2.4.103"},
			{ TS::MPEG4HighProfileLevel4_2_For2DVideo,  "1.2.840.10008.1.2.4.104"},
			{ TS::MPEG4HighProfileLevel4_2_For3DVideo,  "1.2.840.10008.1.2.4.105"},
			{ TS::MPEG4StereoHighProfileLevel4_2,       "1.2.840.10008.1.2.4.106"},
			{ TS::RLELossless,                          "1.2.840.10008.1.2.4.5"}
		};

		return transferSyntaxes;
	}

	using TransferSyntaxMap = boost::container::flat_map<std::string, emds::TransferSyntax>;
	const TransferSyntaxMap& getTransferSyntaxMap()
	{
		static TransferSyntaxMap transferSyntaxMap = [] {
			TransferSyntaxMap tsm;
			const auto& tsp = getTransferSyntaxPairs();
			tsm.reserve(tsp.size());
			for (const auto& p : tsp)
				tsm[p.second] = p.first;
			return tsm;
		}();

		return transferSyntaxMap;
	}

}

namespace emds
{

	TransferSyntax getTransferSyntax(const std::string& uid)
	{
		const auto& tsm = getTransferSyntaxMap();
		auto it = tsm.find(uid);
		if (it != tsm.end())
			return it->second;
		return TransferSyntax::Unknown;
	}

	const std::string& getTransferSyntaxUID(TransferSyntax transferSyntax)
	{
		const auto& ts = getTransferSyntaxPairs();
		int index = static_cast<int>(transferSyntax);
		return ts[index].second;
	}

	bool isImplicit(TransferSyntax ts)
	{
		if (ts == TransferSyntax::Unknown)
			return false;
		return ts == TransferSyntax::ImplicitVRLittleEndian;
	}

	bool isExplicit(TransferSyntax ts)
	{
		if (ts == TransferSyntax::Unknown)
			return false;
		return ts != TransferSyntax::ImplicitVRLittleEndian;
	}

	bool isEncapsulated(TransferSyntax ts)
	{
		switch (ts)
		{
		case TransferSyntax::JPEGBaselineProcess1:
		case TransferSyntax::JPEGExtendedProcess2_4:
		case TransferSyntax::JPEGExtendedProcess3_5:
		case TransferSyntax::JPEGSpectralSelectionProcess6_8:
		case TransferSyntax::JPEGFullProgressionProcess10_12:
		case TransferSyntax::JPEGLosslessProcess14:
		case TransferSyntax::JPEGLosslessProcess14SV1:
		case TransferSyntax::JPEGLSLossless:
		case TransferSyntax::JPEGLSLossy:
		case TransferSyntax::JPEG2000Lossless:
		case TransferSyntax::JPEG2000:
		case TransferSyntax::JPEG2000Part2Lossless:
		case TransferSyntax::JPEG2000Part2:
		case TransferSyntax::JPIPReferenced:
		case TransferSyntax::JPIPReferencedDeflate:
		case TransferSyntax::MPEG2MainProfile:
		case TransferSyntax::MPEG2MainProfileHighLevel:
		case TransferSyntax::MPEG4HighProfileLevel4_1:
		case TransferSyntax::MPEG4BDcompatibleHighProfileLevel4_1:
		case TransferSyntax::MPEG4HighProfileLevel4_2_For2DVideo:
		case TransferSyntax::MPEG4HighProfileLevel4_2_For3DVideo:
		case TransferSyntax::MPEG4StereoHighProfileLevel4_2:
		case TransferSyntax::RLELossless:
			return true;

		default:
			return false;
		}
	}
/*
	bool isLossy(TransferSyntax ts)
	{

	}

	bool isLossless(TransferSyntax ts)
	{

	}
	*/
}
