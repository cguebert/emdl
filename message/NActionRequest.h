#pragma once

#include "Request.h"

namespace emdl
{

namespace message
{

	//! N-ACTION-RQ message.
	class EMDL_API NActionRequest : public Request
	{
	public:
		//! Create an action request with given Message ID, requested SOP class, requested SOP instance and action type ID.
		NActionRequest(Value::Integer messageId, SOP_Class requestedSopClass, SOP_Class requestedSopInstance, Value::Integer actionTypeId);

		//! Create a N-ACTION-RQ from a generic Message.
		NActionRequest(const Message& message);

		MandatoryField<SOP_Class> requestedSopClass;
		MandatoryField<SOP_Class> requestedSopInstance;
		MandatoryField<Value::Integer> actionTypeId;
	
	private:
		NActionRequest();
	};

}

}
