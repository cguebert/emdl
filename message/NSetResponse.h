#pragma once

#include "Response.h"

namespace emdl
{

namespace message
{

	//! N-SET-RSP message.
	class EMDL_API NSetResponse : public Response
	{
	public:
		//! Create a set response with given Message ID and status.
		NSetResponse(Value::Integer messageIdBeingRespondedTo, Value::Integer status);

		//! Create a N-SET-RSP from a generic Message.
		NSetResponse(const Message& message);
	
		OptionalField<SOP_Class> affectedSopClass;
		OptionalField<Value::String> affectedSopInstance;

	private:
		NSetResponse();
	};

}

}

