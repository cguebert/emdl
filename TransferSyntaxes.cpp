#include <emdl/TransferSyntaxes.h>

#include <boost/container/flat_map.hpp>

#include <algorithm>
#include <vector>

namespace
{
	struct TransferSyntaxInfo
	{
		emdl::TransferSyntax code;
		std::string uid, description;
	};
	using TransferSyntaxInfos = std::vector<TransferSyntaxInfo>;
	const TransferSyntaxInfos& getTransferSyntaxInfos()
	{
		// clang-format off
		using TS = emdl::TransferSyntax;
		static TransferSyntaxInfos transferSyntaxes = {
			{ TS::Unknown,                              "Not supported",           "Not supported" },
			{ TS::ImplicitVRLittleEndian,               "1.2.840.10008.1.2",       "Implicit VR Little Endian" },
			{ TS::ExplicitVRLittleEndian,               "1.2.840.10008.1.2.1",     "Explicit VR Little Endian" },
			{ TS::DeflatedExplicitVRLittleEndian,       "1.2.840.10008.1.2.1.99",  "Deflated Explicit VR Little Endian" },
			{ TS::JPEGBaselineProcess1,					"1.2.840.10008.1.2.4.50",  "JPEG Baseline" },
			{ TS::JPEGExtendedProcess2_4,				"1.2.840.10008.1.2.4.51",  "JPEG Extended" },
			{ TS::JPEGExtendedProcess3_5,				"1.2.840.10008.1.2.4.52",  "JPEG Extended (Processes 3 & 5)" },
			{ TS::JPEGSpectralSelectionProcess6_8,		"1.2.840.10008.1.2.4.53",  "JPEG Spectral Selection" },
			{ TS::JPEGFullProgressionProcess10_12,		"1.2.840.10008.1.2.4.55",  "JPEG Full Progression" },
			{ TS::JPEGLosslessProcess14,				"1.2.840.10008.1.2.4.57",  "JPEG Lossless" },
			{ TS::JPEGLosslessProcess14SV1,				"1.2.840.10008.1.2.4.70",  "JPEG Lossless First-Order" },
			{ TS::JPEGLSLossless,                       "1.2.840.10008.1.2.4.80",  "JPEG-LS Lossless" },
			{ TS::JPEGLSLossy,                          "1.2.840.10008.1.2.4.81",  "JPEG-LS Near-Lossless" },
			{ TS::JPEG2000Lossless,						"1.2.840.10008.1.2.4.90",  "JPEG 2000 Lossless" },
			{ TS::JPEG2000,                             "1.2.840.10008.1.2.4.91",  "JPEG 2000 Lossy" },
			{ TS::JPEG2000Part2Lossless,				"1.2.840.10008.1.2.4.92",  "JPEG 2000 Part 2 Lossless" },
			{ TS::JPEG2000Part2,                        "1.2.840.10008.1.2.4.93",  "JPEG 2000 Part 2 Lossy" },
			{ TS::JPIPReferenced,                       "1.2.840.10008.1.2.4.94",  "JPIP Referenced" },
			{ TS::JPIPReferencedDeflate,                "1.2.840.10008.1.2.4.95",  "JPIP Referenced Deflate" },
			{ TS::MPEG2MainProfile,						"1.2.840.10008.1.2.4.100", "MPEG2 Main Profile Main Level" },
			{ TS::MPEG2MainProfileHighLevel,			"1.2.840.10008.1.2.4.101", "MPEG2 Main Profile High Level" },
			{ TS::MPEG4HighProfileLevel4_1,             "1.2.840.10008.1.2.4.102", "MPEG-4 High Profile" },
			{ TS::MPEG4BDcompatibleHighProfileLevel4_1, "1.2.840.10008.1.2.4.103", "MPEG-4 BD-compatible High Profile" },
			{ TS::MPEG4HighProfileLevel4_2_For2DVideo,  "1.2.840.10008.1.2.4.104", "MPEG-4 High Profile For 2D Video" },
			{ TS::MPEG4HighProfileLevel4_2_For3DVideo,  "1.2.840.10008.1.2.4.105", "MPEG-4 High Profile For 3D Video" },
			{ TS::MPEG4StereoHighProfileLevel4_2,       "1.2.840.10008.1.2.4.106", "MPEG-4 Stereo High Profile" },
			{ TS::RLELossless,                          "1.2.840.10008.1.2.4.5",   "RLE Lossless" }
		};
		// clang-format on

		return transferSyntaxes;
	}

	using TransferSyntaxMap = boost::container::flat_map<std::string, emdl::TransferSyntax>;
	const TransferSyntaxMap& getTransferSyntaxMap()
	{
		static TransferSyntaxMap transferSyntaxMap = [] {
			TransferSyntaxMap tsm;
			const auto& tsi = getTransferSyntaxInfos();
			tsm.reserve(tsi.size());
			for (const auto& i : tsi)
				tsm[i.uid] = i.code;
			return tsm;
		}();

		return transferSyntaxMap;
	}
}

namespace emdl
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
		const auto& tsi = getTransferSyntaxInfos();
		int index = static_cast<int>(transferSyntax);
		return tsi[index].uid;
	}

	const std::string& getTransferSyntaxDescription(TransferSyntax transferSyntax)
	{
		const auto& tsi = getTransferSyntaxInfos();
		int index = static_cast<int>(transferSyntax);
		return tsi[index].description;
	}

	std::vector<TransferSyntax> getTransferSyntaxList()
	{
		static const auto list = [] {
			std::vector<TransferSyntax> list;
			const auto& tsi = getTransferSyntaxInfos();
			list.reserve(tsi.size());
			for (const auto& i : tsi)
				list.push_back(i.code);
			return list;
		}();
		return list;
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
