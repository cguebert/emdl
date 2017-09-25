#include <emdl/types/DateTime.h>
#include <emdl/Exception.h>

#include <clocale>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <fmt/format.h>

#ifdef WIN32
#define sscanf sscanf_s
#endif

namespace emdl
{
	namespace types
	{
		DateTime::DateTime() = default;

		DateTime::DateTime(unsigned int year, unsigned int month, unsigned int day,
						   unsigned int hours, unsigned int minutes, double seconds, double timeZone)
			: m_date(year, month, day)
			, m_time(hours, minutes, seconds, timeZone)
		{
		}

		DateTime::DateTime(std::string dateTime)
		{
			if (dateTime.size() < 8)
				throw emdl::Exception("Error parsing a DT value: {}", dateTime);

			// Check if there is a time zone
			double timeZone = 0;
			std::string timeZoneStr = dateTime.substr(dateTime.size() - 5);
			if (timeZoneStr.size() == 5 && ((timeZoneStr[0] == '+') || (timeZoneStr[0] == '-')))
			{
				int hours;
				unsigned int minutes;
				if (sscanf(timeZoneStr.c_str(), "%03i%02u", &hours, &minutes) == 2)
				{
					timeZone = (hours > 0) ? (hours + minutes / 60.0) : (hours - minutes / 60.0);
					dateTime = dateTime.substr(0, dateTime.size() - 5);
				}
				else
					throw emdl::Exception("Error parsing the time zone of a DT value: {}", dateTime);
			}

			unsigned int year, month, day, hours = 0, minutes = 0;
			double seconds = 0;

			// Format: YYYYMMDDHHMM[SS[.FFFFFF]]
			if (sscanf(dateTime.c_str(), "%04u%02u%02u%02u%02u", &year, &month, &day, &hours, &minutes) < 5)
				throw emdl::Exception("Error parsing a DT value: {}", dateTime);

			// Seconds
			if (dateTime.size() > 12)
			{
				dateTime.erase(0, 12);

				// We replace the '.' with the current locale decimal point character, so we can use atof
				auto pos = dateTime.find('.');
				if (pos != std::string::npos)
					dateTime[pos] = *std::localeconv()->decimal_point;

				char* e;
				seconds = std::strtod(dateTime.c_str(), &e);
				if (*e != '\0' || errno)
					throw emdl::Exception("Error parsing the seconds of a TM value: {}", dateTime);
			}

			set(year, month, day, hours, minutes, seconds, timeZone);
		}

		void DateTime::set(unsigned int year, unsigned int month, unsigned int day,
						   unsigned int hours, unsigned int minutes, double seconds, double timezone)
		{
			m_date.set(year, month, day);
			m_time.set(hours, minutes, seconds, timezone);
		}

		void DateTime::setDate(const Date& date)
		{
			m_date = date;
		}

		void DateTime::setTime(const Time& time)
		{
			m_time = time;
		}

		const Date& DateTime::date() const
		{
			return m_date;
		}

		const Time& DateTime::time() const
		{
			return m_time;
		}

		std::string DateTime::isoDateTime() const
		{
			auto dateTimeStr = m_date.isoDate() + "T" + m_time.isoTime();

			// Add the time zone
			auto timeZone = m_time.timeZone();
			if (!timeZone)
				return dateTimeStr;

			const char sign = (timeZone < 0) ? '-' : '+';
			timeZone = std::abs(timeZone);
			const int hours = static_cast<int>(timeZone);
			const unsigned int minutes = static_cast<unsigned int>((timeZone - hours) * 60);
			auto timeZoneStr = fmt::format("{}{:02d}:{:02d}", sign, hours, minutes);
			return dateTimeStr + timeZoneStr;
		}

		std::string DateTime::dicomDateTime() const
		{
			auto dateTimeStr = m_date.dicomDate() + m_time.dicomTime();

			// Add the time zone
			auto timeZone = m_time.timeZone();
			if (!timeZone)
				return dateTimeStr;

			const char sign = (timeZone < 0) ? '-' : '+';
			timeZone = std::abs(timeZone);
			const int hours = static_cast<int>(timeZone);
			const unsigned int minutes = static_cast<unsigned int>((timeZone - hours) * 60);
			auto timeZoneStr = fmt::format("{}{:02d}{:02d}", sign, hours, minutes);
			return dateTimeStr + timeZoneStr;
		}

		DateTime DateTime::currentDateTime()
		{
			using namespace std;
			using namespace std::chrono;

			system_clock::time_point now = system_clock::now();
			system_clock::duration tp = now.time_since_epoch();
			tp -= duration_cast<seconds>(tp); // Keep only the milliseconds part of the time
			const double milli = duration_cast<milliseconds>(tp).count() / 1000.0;

			time_t tt = system_clock::to_time_t(now);
			const std::tm t = *std::localtime(&tt);
			return DateTime(t.tm_year + 1900, t.tm_mon + 1, t.tm_mday,
							t.tm_hour, t.tm_min, t.tm_sec + milli);
		}
	}
}
