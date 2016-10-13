#pragma once

#include <emdl/TransferSyntaxes.h>

#include <odil/Tag.h>
#include <odil/VR.h>

namespace emdl
{

	class SparseDataSet;

	EMDL_API odil::VR asVR(const odil::Tag& tag);
	EMDL_API odil::VR findVR(const odil::Tag& tag, const SparseDataSet& dataSet);

} // namespace emdl
