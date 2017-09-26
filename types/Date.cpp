#include <emdl/types/Date.h>
#include <emdl/Exception.h>

#include <ctime>
#include <chrono>
#include <tuple>
#include <fmt/format.h>

#ifdef WIN32
#define sscanf sscanf_s
#endif

namespace emdl
{
	namespace types
	{
		Date::Date() = default;

		Date::Date(int year, unsigned int month, unsigned int day)
			: m_year(year)
			, m_month(month)
			, m_day(day)
		{
		}

		Date::Date(const std::string& date)
		{
			auto len = date.size();
			if (len == 8) // YYYYMMDD
			{
				if (sscanf(date.c_str(), "%04u%02u%02u", &m_year, &m_month, &m_day) != 3)
					throw emdl::Exception("Error parsing a DA value: {}", date);
			}
			else if (len == 10) // YYYY-MM-DD with any delimiter
			{
				if (sscanf(date.c_str(), "%04u%*c%02u%*c%02u", &m_year, &m_month, &m_day) != 3)
					throw emdl::Exception("Error parsing a DA value: {}", date);
			}
			else
				throw emdl::Exception("Error parsing a DA value: {}", date);
		}

		void Date::set(int year, unsigned int month, unsigned int day)
		{
			m_year = year;
			m_month = month;
			m_day = day;
		}

		void Date::setYear(int year)
		{
			m_year = year;
		}

		void Date::setMonth(unsigned int month)
		{
			m_month = month;
		}

		void Date::setDay(unsigned int day)
		{
			m_day = day;
		}

		int Date::year() const
		{
			return m_year;
		}

		unsigned int Date::month() const
		{
			return m_month;
		}

		unsigned int Date::day() const
		{
			return m_day;
		}

		std::string Date::isoDate() const
		{
			return fmt::format("{:04d}-{:02d}-{:02}", m_year, m_month, m_day);
		}

		std::string Date::dicomDate() const
		{
			return fmt::format("{:04d}{:02d}{:02d}", m_year, m_month, m_day);
		}

		Date Date::currentDate()
		{
			using namespace std;
			using namespace std::chrono;

			system_clock::time_point now = system_clock::now();
			time_t tt = system_clock::to_time_t(now);
			const std::tm t = *std::localtime(&tt);
			return Date(t.tm_year + 1900, t.tm_mon + 1, t.tm_mday);
		}

		bool operator<(const Date& lhs, const Date& rhs)
		{
			return std::tuple<unsigned int, unsigned int, unsigned int>(lhs.year(), lhs.month(), lhs.day())
				   < std::tuple<unsigned int, unsigned int, unsigned int>(rhs.year(), rhs.month(), rhs.day());
		}

		bool operator>(const Date& lhs, const Date& rhs)
		{
			return rhs < lhs;
		}

		bool operator<=(const Date& lhs, const Date& rhs)
		{
			return !(lhs > rhs);
		}

		bool operator>=(const Date& lhs, const Date& rhs)
		{
			return !(lhs < rhs);
		}

		bool operator==(const Date& lhs, const Date& rhs)
		{
			return lhs.year() == rhs.year()
				   && lhs.month() == rhs.month()
				   && lhs.day() == rhs.day();
		}

		bool operator!=(const Date& lhs, const Date& rhs)
		{
			return !(lhs == rhs);
		}
	}
}
