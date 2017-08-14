#pragma once

#include <emdl/pdu/items/BaseItem.h>
#include <emdl/pdu/ObjectStringField.h>

namespace emdl
{
	namespace pdu
	{
		/// User Identity Sub-Item Structure (A-ASSOCIATE-RQ) (PS 3.7, D.3.3.7.1).
		class EMDL_API UserIdentityRQ : public BaseItem
		{
		public:
			UserIdentityRQ();
			UserIdentityRQ(std::istream& stream);

			uint8_t type() const;
			void setType(uint8_t type);

			bool positiveResponseRequested() const;
			void setPositiveResponseRequested(bool value);

			const std::string& primaryField() const;
			void setPrimaryField(const std::string& value);

			const std::string& secondaryField() const;
			void setSecondaryField(const std::string& value);

			// Helpers
			void setUsername(const std::string& username);
			void setUsernameAndPasscode(const std::string& username, const std::string& passcode);
			void setKerberosServiceTicket(const std::string& ticket);
			void setSamlAssertion(const std::string& assertion);

		private:
			Field<uint8_t> m_type;
			Field<uint8_t> m_positiveResponseRequested;
			Field<uint16_t> m_primaryFieldLength;
			StringField<uint16_t> m_primaryField;
			Field<uint16_t> m_secondaryFieldLength;
			StringField<uint16_t> m_secondaryField;
		};
	}
}
