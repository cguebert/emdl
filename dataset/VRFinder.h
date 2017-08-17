#pragma once

#include <emdl/dataset/VR.h>
#include <emdl/TransferSyntaxes.h>

#include <odil/Tag.h>

namespace emdl
{
	class SparseDataSet;

	EMDL_API VR findVR(const odil::Tag& tag);
	EMDL_API VR findVR(const odil::Tag& tag, const SparseDataSet& dataSet);

} // namespace emdl
