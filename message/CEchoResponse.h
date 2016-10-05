#pragma once

#include "Response.h"

namespace emdl
{

namespace message
{

	//! C-ECHO-RSP message.
	class CEchoResponse : public Response
	{
	public:
		//! Create an echo response with given Message ID and affected SOP class UID.
		CEchoResponse(Value::Integer messageIdBeingRespondedTo, Value::Integer status, const Value::String& affectedSopClassUid);

		//! Create a C-ECHO-RSP from a generic Message.
		CEchoResponse(const Message& message);
	
		MandatoryField<Value::String> affectedSopClassUid;

	private:
		CEchoResponse();
	};

}

}

