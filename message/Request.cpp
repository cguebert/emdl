#include <emdl/message/Request.h>
#include <emdl/registry.h>

namespace emdl
{
	namespace message
	{
		Request::Request()
			: messageId(initField(registry::MessageID))
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
