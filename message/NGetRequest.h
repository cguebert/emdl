#pragma once

#include <emdl/message/Request.h>

namespace emdl
{
	namespace message
	{
		//! N-GET-RQ message.
		class EMDL_API NGetRequest : public Request
		{
		public:
			//! Create a get request with given Message ID, requested SOP class and requested SOP instance.
			NGetRequest(Value::Integer messageId, SOP_Class requestedSopClass, Value::String requestedSopInstance);

			//! Create a N-GET-RQ from a generic Message.
			NGetRequest(const Message& message);

			MandatoryField<SOP_Class> requestedSopClass;
			MandatoryField<Value::String> requestedSopInstance;
			OptionalField<Value::Strings> attributeIdentifierList;

		private:
			NGetRequest();
		};
	}
}
