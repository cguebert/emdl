#pragma once

#include <string>

namespace emds
{

	// We removed completely all traces of the big endian TS, to simplify a lot of code
	enum class TransferSyntax
	{
		Unknown, /// Not supported, or not recognized
		ImplicitVRLittleEndian,
		ExplicitVRLittleEndian,
		DeflatedExplicitVRLittleEndian,
		JPEGBaselineProcess1,	// JPEG Baseline (8 bits compressed)
		JPEGExtendedProcess2_4,	// JPEG Extended (12 bits compressed)
		JPEGExtendedProcess3_5,
		JPEGSpectralSelectionProcess6_8,
		JPEGFullProgressionProcess10_12,
		JPEGLosslessProcess14,	// JPEG Lossless
		JPEGLosslessProcess14SV1,	// JPEG Lossless, first order prediction (default for jpeg lossless)
		JPEGLSLossless,
		JPEGLSLossy,
		JPEG2000Lossless,
		JPEG2000,
		JPEG2000Part2Lossless,
		JPEG2000Part2,
		JPIPReferenced,
		JPIPReferencedDeflate,
		MPEG2MainProfile,
		MPEG2MainProfileHighLevel,
		MPEG4HighProfileLevel4_1,
		MPEG4BDcompatibleHighProfileLevel4_1,
		MPEG4HighProfileLevel4_2_For2DVideo,
		MPEG4HighProfileLevel4_2_For3DVideo,
		MPEG4StereoHighProfileLevel4_2,
		RLELossless
	};

	TransferSyntax getTransferSyntax(const std::string& uid);
	const std::string& getTransferSyntaxUID(TransferSyntax transferSyntax);

	bool isImplicit(TransferSyntax ts);
	bool isExplicit(TransferSyntax ts);
	bool isEncapsulated(TransferSyntax ts);
//	bool isLossy(TransferSyntax ts);
//	bool isLossless(TransferSyntax ts);

}
