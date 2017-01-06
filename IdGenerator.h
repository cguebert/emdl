#pragma once

#include <emdl/emdl_api.h>

#include <string>

namespace emdl
{

	// Returns the end of an uid
	// It uses the format: a.b with
	// a = number of seconds since 01/01/2000
	// b = auto increment absolute counter (starts at a random value and wraps at 2^32)
	EMDL_API std::string getUidSuffix();

}
