#include "Exception.h"

namespace emds
{

	Exception::Exception(const std::string& message)
		: m_message(message) 
	{
	}

	Exception::~Exception() 
	{
	}

	const char* Exception::what() const
	{
		return m_message.c_str();
	}

}
