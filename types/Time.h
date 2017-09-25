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
			Time(unsigned int hours, unsigned int minutes, double seconds, double timeZone = 0);
			explicit Time(const std::string time);

			void set(unsigned int hours, unsigned int minutes, double seconds, double timeZone = 0);
			void setHours(unsigned int hours);
			void setMinutes(unsigned int minutes);
			void setSeconds(double seconds);
			void setTimeZone(double timeZone);

			unsigned int hours() const;
			unsigned int minutes() const;
			double seconds() const;
			double timeZone() const;

			std::string isoTime() const;
			std::string dicomTime() const;

			static Time currentTime();

		private:
			unsigned int m_hours = 0, m_minutes = 0;
			double m_seconds = 0, m_timeZone = 0;
		};
	}
}
