#pragma once

#include <emdl/emdl_api.h>

namespace emdl
{
	/// Write a double as a DS to the buffer.
	EMDL_API void writeDS(double f, char* buffer, int size = 16);
}
