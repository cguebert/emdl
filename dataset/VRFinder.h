#pragma once

#include <emdl/dataset/VR.h>
#include <emdl/TransferSyntaxes.h>

#include <odil/Tag.h>

namespace emdl
{
	class DataSet;

	EMDL_API VR findVR(const odil::Tag& tag);
	EMDL_API VR findVR(const odil::Tag& tag, const DataSet& dataSet);

} // namespace emdl
