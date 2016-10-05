#include "Request.h"

#include <odil/registry.h>

namespace emdl
{

namespace message
{

	Request::Request()
		: messageId(initField(odil::registry::MessageID))
	{
	}

	Request::Request(Value::Integer msgId)
		: Request()
	{
		messageId.set(msgId);
	}

	Request::Request(const Message& message)
		: Request()
	{
		copyFieldsFromMessage(message);
	}

}

}
