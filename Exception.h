#pragma once

#include <exception>
#include <string>

#include <fmt/format.h>

#ifndef LOG_POSITION
#define EMDSLOG_STR_H(x) #x
#define EMDSLOG_STR_HELPER(x) EMDSLOG_STR_H(x)
#define LOG_POSITION ("[" __FILE__ " line " EMDSLOG_STR_HELPER(__LINE__) "] ")
#endif

namespace emds
{

	//! Base class for EMDS exceptions, allowing use of std::string
	class Exception : public std::exception
	{
	public:
		Exception(const std::string& message = ""); //!< Message string constructor.

		template <class ...Args> 
		Exception(const std::string& str, Args&&... args)
			: m_message(fmt::format(str, std::forward<Args>(args)...))
		{}

		virtual ~Exception(); //!< Destructor.

		const char* what() const override; //!< Return the reason for the exception.

	protected:
		std::string m_message; //!< brief Message of the exception.
	};

}
