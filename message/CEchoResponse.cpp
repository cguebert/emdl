#include <emdl/message/CEchoResponse.h>
#include <emdl/registry.h>

namespace emdl
{
	namespace message
	{
		CEchoResponse::CEchoResponse()
			: affectedSopClass(initField(registry::AffectedSOPClassUID))
		{
			commandField.set(Command::C_ECHO_RSP);
		}

		CEchoResponse::CEchoResponse(Value::Integer msgIdResponse, Value::Integer statusVal, SOP_Class sopClass)
			: CEchoResponse()
		{
			messageIdBeingRespondedTo.set(msgIdResponse);
			status.set(statusVal);
			affectedSopClass.set(sopClass);
		}

		CEchoResponse::CEchoResponse(const Message& message)
			: CEchoResponse()
		{
			copyFromMessage(message, Command::C_ECHO_RSP, DataSetRequirement::Ignore);
		}
	}
}
