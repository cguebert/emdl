#pragma once

#include <emdl/emdl_api.h>
#include <string>

namespace emdl
{
	namespace types
	{
		//! Class representing the VR TM
		class EMDL_API Time
		{
		public:
			Time();
			Time(unsigned int hours, unsigned int minutes, double seconds, int timeZone = 0);
			explicit Time(const std::string time);

			void set(unsigned int hours, unsigned int minutes, double seconds, int timeZone = 0);
			void setHours(unsigned int hours);
			void setMinutes(unsigned int minutes);
			void setSeconds(double seconds);
			void setTimeZone(int timeZone);

			unsigned int hours() const;
			unsigned int minutes() const;
			double seconds() const;
			int timeZone() const;

			std::string isoTime() const;
			std::string dicomTime() const;

			static Time currentTime();

		private:
			unsigned int m_hours = 0, m_minutes = 0;
			double m_seconds = 0;
			int m_timeZone = 0; // Delta, in minutes
		};

		EMDL_API bool operator<(const Time& lhs, const Time& rhs);
		EMDL_API bool operator>(const Time& lhs, const Time& rhs);
		EMDL_API bool operator<=(const Time& lhs, const Time& rhs);
		EMDL_API bool operator>=(const Time& lhs, const Time& rhs);
		EMDL_API bool operator==(const Time& lhs, const Time& rhs);
		EMDL_API bool operator!=(const Time& lhs, const Time& rhs);
	}
}
