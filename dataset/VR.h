#pragma once

#include <emdl/emdl_api.h>

#include <string>

namespace emdl
{
	/// Value representations of DICOM.
	enum class VR
	{
		// clang-format off
		Unknown, // Not set, must look for the VR corresponding to a Tag
		AE, AS, AT, CS, DA, DS, DT, FL, FD, IS, LO, LT, OB, OD, OF, OL, OW, PN, SH, 
		SL, SQ, SS, ST, TM, UC, UI, UL, UN, UR, US, UT,
		Invalid // Error value
		// clang-format on
	};

	enum class VRType
	{
		Int,
		Real,
		String,
		Binary,
		Dataset
	};

	/// Convert a VR to its string representation.
	EMDL_API std::string asString(VR vr);

	/// Convert a string to its VR, or throw an exception.
	EMDL_API VR asVr(const std::string& vr);

	/// Return the data type of the VR
	EMDL_API VRType vrType(VR vr);
}
