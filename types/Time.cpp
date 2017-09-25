#include <emdl/types/Time.h>
#include <emdl/Exception.h>

#include <clocale>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <tuple>
#include <errno.h>
#include <fmt/format.h>

#ifdef WIN32
#define sscanf sscanf_s
#endif

namespace emdl
{
	namespace types
	{
		Time::Time() = default;

		Time::Time(unsigned int hours, unsigned int minutes, double seconds, int timeZone)
			: m_hours(hours)
			, m_minutes(minutes)
			, m_seconds(seconds)
			, m_timeZone(timeZone)
		{
		}

		Time::Time(std::string timeStr)
		{
			// Remove ':'
			if ((timeStr.size() > 5) && (timeStr[5] == ':'))
				timeStr.erase(5, 1);
			if ((timeStr.size() > 2) && (timeStr[2] == ':'))
				timeStr.erase(2, 1);

			// Format is HH[MM[SS[.FFFFFF]]]
			if (sscanf(timeStr.c_str(), "%02u%02u", &m_hours, &m_minutes) < 1)
				throw emdl::Exception("Error parsing a TM value: {}", timeStr);

			// Parse seconds as double
			if (timeStr.size() > 4)
			{
				timeStr.erase(0, 4);

				// We replace the '.' with the current locale decimal point character, so we can use atof
				auto pos = timeStr.find('.');
				if (pos != std::string::npos)
					timeStr[pos] = *std::localeconv()->decimal_point;

				char* e;
				m_seconds = std::strtod(timeStr.c_str(), &e);
				if (*e != '\0' || errno)
					throw emdl::Exception("Error parsing the seconds of a TM value: {}", timeStr);
			}
		}

		void Time::set(unsigned int hours, unsigned int minutes, double seconds, int timeZone)
		{
			m_hours = hours;
			m_minutes = minutes;
			m_seconds = seconds;
			m_timeZone = timeZone;
		}

		void Time::setHours(unsigned int hours)
		{
			m_hours = hours;
		}

		void Time::setMinutes(unsigned int minutes)
		{
			m_minutes = minutes;
		}

		void Time::setSeconds(double seconds)
		{
			m_seconds = seconds;
		}

		void Time::setTimeZone(int timeZone)
		{
			m_timeZone = timeZone;
		}

		unsigned int Time::hours() const
		{
			return m_hours;
		}

		unsigned int Time::minutes() const
		{
			return m_minutes;
		}

		double Time::seconds() const
		{
			return m_seconds;
		}

		int Time::timeZone() const
		{
			return m_timeZone;
		}

		std::string Time::isoTime() const
		{
			return fmt::format("{:02d}:{:02d}:{:g}", m_hours, m_minutes, m_seconds);
		}

		std::string Time::dicomTime() const
		{
			return fmt::format("{:02d}{:02d}{:g}", m_hours, m_minutes, m_seconds);
		}

		Time Time::currentTime()
		{
			using namespace std;
			using namespace std::chrono;

			system_clock::time_point now = system_clock::now();
			system_clock::duration tp = now.time_since_epoch();
			tp -= duration_cast<chrono::seconds>(tp); // Keep only the milliseconds part of the time
			const double milli = duration_cast<milliseconds>(tp).count() / 1000.0;

			time_t tt = system_clock::to_time_t(now);
			const std::tm t = *std::localtime(&tt);
			return Time(t.tm_hour, t.tm_min, t.tm_sec + milli);
		}

		bool operator<(const Time& lhs, const Time& rhs)
		{
			double l = (lhs.hours() * 60 + lhs.minutes() - lhs.timeZone()) * 60 + lhs.seconds();
			double r = (rhs.hours() * 60 + rhs.minutes() - rhs.timeZone()) * 60 + rhs.seconds();

			return l < r;
		}

		bool operator>(const Time& lhs, const Time& rhs)
		{
			return rhs < lhs;
		}

		bool operator<=(const Time& lhs, const Time& rhs)
		{
			return !(lhs > rhs);
		}

		bool operator>=(const Time& lhs, const Time& rhs)
		{
			return !(lhs < rhs);
		}

		bool operator==(const Time& lhs, const Time& rhs)
		{
			const int lm = lhs.hours() * 60 + lhs.minutes() - lhs.timeZone();
			const int rm = rhs.hours() * 60 + rhs.minutes() - rhs.timeZone();

			return lm == rm
				   && lhs.seconds() == rhs.seconds();
		}

		bool operator!=(const Time& lhs, const Time& rhs)
		{
			return !(lhs == rhs);
		}
	}
}
