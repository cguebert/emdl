#pragma once

#include <emdl/emdl_api.h>

#include <string>

namespace emdl
{

	// We removed completely all traces of the big endian TS, to simplify a lot of code
	enum class EMDL_API TransferSyntax
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

	EMDL_API TransferSyntax getTransferSyntax(const std::string& uid);
	EMDL_API const std::string& getTransferSyntaxUID(TransferSyntax transferSyntax);
	EMDL_API const std::string& getTransferSyntaxDescription(TransferSyntax transferSyntax);

	EMDL_API bool isImplicit(TransferSyntax ts);
	EMDL_API bool isExplicit(TransferSyntax ts);
	EMDL_API bool isEncapsulated(TransferSyntax ts);
//	EMDL_API bool isLossy(TransferSyntax ts);
//	EMDL_API bool isLossless(TransferSyntax ts);

}
