#pragma once

#include "Request.h"

namespace emds
{

namespace message
{

	//! C-ECHO-RQ message.
	class CEchoRequest : public Request
	{
	public:
		//! Create an echo request with given Message ID and affected SOP class UID.
		CEchoRequest(Value::Integer messageId, const Value::String& affectedSopClassUid);

		//! Create a C-ECHO-RQ from a generic Message.
		CEchoRequest(const Message& message);

		MandatoryField<Value::String> affectedSopClassUid;
	
	private:
		CEchoRequest();
	};

}

}
