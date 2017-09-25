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
			Date(unsigned int year, unsigned int month, unsigned int day);
			explicit Date(const std::string& date);

			void set(unsigned int year, unsigned int month, unsigned int day);
			void setYear(unsigned int year);
			void setMonth(unsigned int month);
			void setDay(unsigned int day);

			unsigned int year() const;
			unsigned int month() const;
			unsigned int day() const;

			std::string isoDate() const;
			std::string dicomDate() const;

			static Date currentDate();

		private:
			unsigned int m_year = 0, m_month = 0, m_day = 0;
		};
	}
}
