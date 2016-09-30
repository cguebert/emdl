#include "CEchoRequest.h"

#include <odil/registry.h>

namespace emds
{

namespace message
{

	CEchoRequest::CEchoRequest()
		: affectedSopClassUid(initField(odil::registry::AffectedSOPClassUID))
	{
		commandField.set(Command::C_ECHO_RQ);
	}

	CEchoRequest::CEchoRequest(Value::Integer msgId, const Value::String& sopClass)
		: CEchoRequest()
	{
		messageId.set(msgId);
		affectedSopClassUid.set(sopClass);
	}

	CEchoRequest::CEchoRequest(const Message& message)
		: CEchoRequest()
	{
		copyFromMessage(message, Command::C_ECHO_RQ, DataSetRequirement::Ignore);
	}

}

}
