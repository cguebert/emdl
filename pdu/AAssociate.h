#pragma once

#include <emdl/pdu/BasePDU.h>
#include <emdl/pdu/ObjectStringField.h>
#include <emdl/pdu/items/ItemsField.h>
#include <emdl/pdu/items/ApplicationContext.h>
#include <emdl/pdu/items/UserInformation.h>

namespace emdl
{
	namespace pdu
	{
		/// A-ASSOCIATE-RQ and A-ASSOCIATE-AC PDU, cf. PS 3.8, 9.3.2 and 9.3.3.
		class EMDL_API AAssociate : public BasePDU
		{
		public:
			AAssociate();
			AAssociate(std::istream& in);

			Field<uint16_t> protocolVersion;
			const Field<uint16_t> reserved2;
			FixedStringField calledAeTitle;
			FixedStringField callingAeTitle;
			const FixedStringField reserved3;

			std::string applicationContext() const;
			void setApplicationContext(const std::string& value);

			std::shared_ptr<UserInformation> userInformation() const;
			void setUserInformation(const std::shared_ptr<UserInformation>& value);

		protected:
			ItemsField<uint32_t> items;
		};
	}
}
