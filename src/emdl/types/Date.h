#pragma once

#include <emdl/emdl_api.h>
#include <string>

namespace emdl
{
	namespace types
	{
		//! Class representing the VR DA
		class EMDL_API Date
		{
		public:
			Date();
			Date(int year, unsigned int month, unsigned int day);
			explicit Date(const std::string& date);

			void set(int year, unsigned int month, unsigned int day);
			void setYear(int year);
			void setMonth(unsigned int month);
			void setDay(unsigned int day);

			int year() const;
			unsigned int month() const;
			unsigned int day() const;

			std::string isoDate() const;
			std::string dicomDate() const;

			static Date currentDate();

		private:
			int m_year = 0;
			unsigned int m_month = 0, m_day = 0;
		};

		EMDL_API bool operator<(const Date& lhs, const Date& rhs);
		EMDL_API bool operator>(const Date& lhs, const Date& rhs);
		EMDL_API bool operator<=(const Date& lhs, const Date& rhs);
		EMDL_API bool operator>=(const Date& lhs, const Date& rhs);
		EMDL_API bool operator==(const Date& lhs, const Date& rhs);
		EMDL_API bool operator!=(const Date& lhs, const Date& rhs);
	}
}
