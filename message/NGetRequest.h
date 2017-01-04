#pragma once

#include "Request.h"

namespace emdl
{

namespace message
{

	//! N-GET-RQ message.
	class EMDL_API NGetRequest : public Request
	{
	public:
		//! Create a get request with given Message ID, requested SOP class and requested SOP instance.
		NGetRequest(Value::Integer messageId, SOP_Class requestedSopClass, SOP_Class requestedSopInstance);

		//! Create a N-GET-RQ from a generic Message.
		NGetRequest(const Message& message);

		MandatoryField<SOP_Class> requestedSopClass;
		MandatoryField<SOP_Class> requestedSopInstance;
		OptionalField<Value::String> attributeIdentifierList;
	
	private:
		NGetRequest();
	};

}

}
