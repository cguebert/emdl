#pragma once

#include <emdl/dataset/VR.h>
#include <emdl/Tag.h>

namespace emdl
{
	class DataSet;

	EMDL_API VR findVR(const Tag tag);
	EMDL_API VR findVR(const Tag tag, const DataSet& dataSet);

} // namespace emdl
