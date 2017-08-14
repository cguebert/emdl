#pragma once

#include <emdl/pdu/items/BaseItem.h>
#include <emdl/pdu/ObjectStringField.h>

namespace emdl
{
	namespace pdu
	{
		/// User Identity Sub-Item Structure (A-ASSOCIATE-AC) (PS 3.7, D.3.3.7.2).
		class EMDL_API UserIdentityAC : public BaseItem
		{
		public:
			UserIdentityAC(const std::string& serverResponse);
			UserIdentityAC(std::istream& in);

			const std::string& serverResponse() const;
			void setServerResponse(const std::string& value);

		private:
			UserIdentityAC();

			Field<uint16_t> m_serverResponseLength;
			StringField<uint16_t> m_serverResponse;
		};
	}
}
