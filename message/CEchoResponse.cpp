#include "CEchoResponse.h"

#include <odil/registry.h>

namespace emdl
{

namespace message
{

	CEchoResponse::CEchoResponse()
		: affectedSopClassUid(initField(odil::registry::AffectedSOPClassUID))
	{
		commandField.set(Command::C_ECHO_RSP);
	}

	CEchoResponse::CEchoResponse(Value::Integer msgIdResponse, Value::Integer statusVal, const Value::String& sopClass)
		: CEchoResponse()
	{
		messageIdBeingRespondedTo.set(msgIdResponse);
		status.set(statusVal);
		affectedSopClassUid.set(sopClass);
	}

	CEchoResponse::CEchoResponse(const Message& message)
		: CEchoResponse()
	{
		copyFromMessage(message, Command::C_ECHO_RSP, DataSetRequirement::Ignore);
	}

}

}
