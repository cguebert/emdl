#pragma once

#include "Request.h"

namespace emdl
{

namespace message
{

	//! N-DELETE-RQ message.
	class EMDL_API NDeleteRequest : public Request
	{
	public:
		//! Create a delete request with given Message ID, requested SOP class and requested SOP instance.
		NDeleteRequest(Value::Integer messageId, SOP_Class requestedSopClass, Value::String requestedSopInstance);

		//! Create a N-DELETE-RQ from a generic Message.
		NDeleteRequest(const Message& message);

		MandatoryField<SOP_Class> requestedSopClass;
		MandatoryField<Value::String> requestedSopInstance;
	
	private:
		NDeleteRequest();
	};

}

}
