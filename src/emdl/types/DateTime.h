#pragma once

#include <emdl/types/Date.h>
#include <emdl/types/Time.h>

namespace emdl
{
	namespace types
	{
		//! Class representing the VR DT
		class EMDL_API DateTime
		{
		public:
			DateTime();
			DateTime(int year, unsigned int month, unsigned int day,
					 unsigned int hours, unsigned int minutes, double seconds, int timeZone = 0);
			explicit DateTime(const std::string dateTime);

			void set(int year, unsigned int month, unsigned int day,
					 unsigned int hours, unsigned int minutes, double seconds, int timeZone = 0);

			void setDate(const Date& date);
			void setTime(const Time& time);

			const Date& date() const;
			const Time& time() const;

			std::string isoDateTime() const;
			std::string dicomDateTime() const;

			static DateTime currentDateTime();

		private:
			Date m_date;
			Time m_time;
		};

		EMDL_API bool operator<(const DateTime& lhs, const DateTime& rhs);
		EMDL_API bool operator>(const DateTime& lhs, const DateTime& rhs);
		EMDL_API bool operator<=(const DateTime& lhs, const DateTime& rhs);
		EMDL_API bool operator>=(const DateTime& lhs, const DateTime& rhs);
		EMDL_API bool operator==(const DateTime& lhs, const DateTime& rhs);
		EMDL_API bool operator!=(const DateTime& lhs, const DateTime& rhs);
	}
}
