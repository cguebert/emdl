#pragma once

#include "Response.h"

namespace emdl
{

namespace message
{

	//! N-ACTION-RSP message.
	class EMDL_API NActionResponse : public Response
	{
	public:
		//! Create an action response with given Message ID and status.
		NActionResponse(Value::Integer messageIdBeingRespondedTo, Value::Integer status);

		//! Create a N-ACTION-RSP from a generic Message.
		NActionResponse(const Message& message);
	
		OptionalField<Value::Integer> actionTypeId;
		OptionalField<SOP_Class> affectedSopClass;
		OptionalField<Value::String> affectedSopInstance;

	private:
		NActionResponse();
	};

}

}

