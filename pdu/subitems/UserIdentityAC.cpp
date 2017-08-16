#include <emdl/pdu/subitems/UserIdentityAC.h>

namespace emdl
{
	namespace pdu
	{
		UserIdentityAC::UserIdentityAC()
			: m_serverResponseLength(initField("Server-response-length", 0))
			, m_serverResponse(initField("Server-response"), m_serverResponseLength, 0)
		{
			itemType.set(ItemType::UserIdentityAC);
		}

		UserIdentityAC::UserIdentityAC(const std::string& serverResponse)
			: UserIdentityAC()
		{
			m_serverResponse.set(serverResponse);
		}

		UserIdentityAC::UserIdentityAC(std::istream& in)
			: UserIdentityAC()
		{
			read(in);
		}

		const std::string& UserIdentityAC::serverResponse() const
		{
			return m_serverResponse.get();
		}

		void UserIdentityAC::setServerResponse(const std::string& value)
		{
			m_serverResponse.set(value);
		}
	}
}
