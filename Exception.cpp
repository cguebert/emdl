#include <emdl/Exception.h>

namespace emdl
{
	Exception::Exception(const std::string& message)
		: m_message(message)
	{
	}

	const char* Exception::what() const
	{
		return m_message.c_str();
	}
}
