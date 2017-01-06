#pragma once

#include "Response.h"

namespace emdl
{

namespace message
{

	//! N-DELETE-RSP message.
	class EMDL_API NDeleteResponse : public Response
	{
	public:
		//! Create a delete response with given Message ID and status.
		NDeleteResponse(Value::Integer messageIdBeingRespondedTo, Value::Integer status);

		//! Create a N-DELETE-RSP from a generic Message.
		NDeleteResponse(const Message& message);
	
		OptionalField<SOP_Class> affectedSopClass;
		OptionalField<Value::String> affectedSopInstance;

	private:
		NDeleteResponse();
	};

}

}

