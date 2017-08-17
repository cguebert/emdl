#include <emdl/message/CEchoRequest.h>
#include <emdl/registry.h>

namespace emdl
{
	namespace message
	{
		CEchoRequest::CEchoRequest()
			: affectedSopClass(initField(registry::AffectedSOPClassUID))
		{
			commandField.set(Command::C_ECHO_RQ);
		}

		CEchoRequest::CEchoRequest(Value::Integer msgId, SOP_Class sopClass)
			: CEchoRequest()
		{
			messageId.set(msgId);
			affectedSopClass.set(sopClass);
		}

		CEchoRequest::CEchoRequest(const Message& message)
			: CEchoRequest()
		{
			copyFromMessage(message, Command::C_ECHO_RQ, DataSetRequirement::Ignore);
		}
	}
}
