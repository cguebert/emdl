#include <emdl/IdGenerator.h>

#include <atomic>
#include <cstring>
#include <ctime>
#include <limits>
#include <memory>
#include <random>

namespace
{
	class IdGenerator
	{
	public:
		static std::string getUid();

	private:
		IdGenerator();

		std::atomic<uint32_t> m_atomicCounter;
		std::time_t m_timeRef;

		std::string seconds();
		std::string counter();
	};

	IdGenerator::IdGenerator()
	{
		/* January 01, 2000 */
		std::tm refDate;
		std::memset(&refDate, 0, sizeof(refDate));
		refDate.tm_mday = 1;
		refDate.tm_year = 100;
		m_timeRef = std::mktime(&refDate);

		std::random_device rd;
		std::default_random_engine engine(rd());
		std::uniform_int_distribution<uint32_t> dist(0, std::numeric_limits<uint32_t>::max());

		m_atomicCounter = static_cast<uint32_t>(dist(engine));
	}

	std::string IdGenerator::getUid()
	{
		static IdGenerator gen;
		return gen.seconds() + '.' + gen.counter();
	}

	std::string IdGenerator::seconds()
	{
		std::time_t now = std::time(nullptr);
		const double diff = std::floor(std::difftime(now, m_timeRef));
		const auto nbSeconds = static_cast<uint32_t>(diff);

		return std::to_string(nbSeconds);
	}

	std::string IdGenerator::counter()
	{
		return std::to_string(m_atomicCounter++); // Unsigned int never overflows (i.e. MAX_INT +1 == 0)
	}
}

namespace emdl
{
	std::string getUidSuffix()
	{
		return IdGenerator::getUid();
	}
}
