#include <emdl/pdu/BasePDU.h>
#include <emdl/Exception.h>

namespace emdl
{
	namespace pdu
	{
		BasePDU::BasePDU()
			: pduType(initField("PDU-type"))
			, reserved1(initField("Reserved-1", 0))
			, length(initField("PDU-length", 0))
		{
		}

		void BasePDU::read(std::istream& in)
		{
			uint8_t type = 0;
			in.read(reinterpret_cast<char*>(&type), 1);

			const auto expected = pduType.get();
			if (type != expected)
				throw Exception("Invalid PDU type: excepted {}, got {}", expected, type);

			auto it = m_fields.begin(), itEnd = m_fields.end();
			std::advance(it, 1); // Ignore the type field
			for (; it != itEnd; ++it)
				(*it)->read(in);
		}
	}
}
