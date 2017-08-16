#include <emdl/pdu/subitems/UserIdentityRQ.h>
#include <emdl/pdu/items/ItemFactory.h>

namespace emdl
{
	namespace pdu
	{
		UserIdentityRQ::UserIdentityRQ()
			: m_type(initField("User-Identity-Type", 1))
			, m_positiveResponseRequested(initField("Positive-response-requested", 0))
			, m_primaryFieldLength(initField("Primary-field-length", 0))
			, m_primaryField(initField("Primary-field"), m_primaryFieldLength, 0)
			, m_secondaryFieldLength(initField("Secondary-field-length", 0))
			, m_secondaryField(initField("Secondary-field"), m_secondaryFieldLength, 0)
		{
			itemType.set(ItemType::UserIdentityRQ);
			length.set(size());
		}

		UserIdentityRQ::UserIdentityRQ(std::istream& in)
			: UserIdentityRQ()
		{
			read(in);
		}

		uint8_t UserIdentityRQ::type() const
		{
			return m_type.get();
		}

		void UserIdentityRQ::setType(uint8_t type)
		{
			m_type.set(type);
		}

		bool UserIdentityRQ::positiveResponseRequested() const
		{
			return m_positiveResponseRequested.get() != 0;
		}

		void UserIdentityRQ::setPositiveResponseRequested(bool value)
		{
			m_positiveResponseRequested.set(value ? 1 : 0);
		}

		const std::string& UserIdentityRQ::primaryField() const
		{
			return m_primaryField.get();
		}

		void UserIdentityRQ::setPrimaryField(const std::string& value)
		{
			m_primaryField.set(value);
			length.set(size());
		}

		const std::string& UserIdentityRQ::secondaryField() const
		{
			return m_secondaryField.get();
		}

		void UserIdentityRQ::setSecondaryField(const std::string& value)
		{
			m_secondaryField.set(value);
			length.set(size());
		}

		void UserIdentityRQ::setUsername(const std::string& username)
		{
			m_type.set(1);
			m_primaryField.set(username);
			m_secondaryField.set("");
		}

		void UserIdentityRQ::setUsernameAndPasscode(const std::string& username, const std::string& passcode)
		{
			m_type.set(2);
			m_primaryField.set(username);
			m_secondaryField.set(passcode);
		}

		void UserIdentityRQ::setKerberosServiceTicket(const std::string& ticket)
		{
			m_type.set(3);
			m_primaryField.set(ticket);
			m_secondaryField.set("");
		}

		void UserIdentityRQ::setSamlAssertion(const std::string& assertion)
		{
			m_type.set(4);
			m_primaryField.set(assertion);
			m_secondaryField.set("");
		}

		RegisterItem<UserIdentityRQ> userIdentityRQSubItem{ItemType::UserIdentityRQ};
	}
}
