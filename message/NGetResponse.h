#pragma once

#include "Response.h"

namespace emdl
{

namespace message
{

	//! N-GET-RSP message.
	class EMDL_API NGetResponse : public Response
	{
	public:
		//! Create a get response with given Message ID and status.
		NGetResponse(Value::Integer messageIdBeingRespondedTo, Value::Integer status);

		//! Create a N-GET-RSP from a generic Message.
		NGetResponse(const Message& message);
	
		OptionalField<SOP_Class> affectedSopClass;
		OptionalField<SOP_Class> affectedSopInstance;

	private:
		NGetResponse();
	};

}

}

