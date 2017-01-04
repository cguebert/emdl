#include "NGetResponse.h"

#include <odil/registry.h>

namespace emdl
{

namespace message
{

	NGetResponse::NGetResponse()
		: affectedSopClass(initField(odil::registry::AffectedSOPClassUID))
		, affectedSopInstance(initField(odil::registry::AffectedSOPInstanceUID))
	{
		commandField.set(Command::N_GET_RSP);
	}

	NGetResponse::NGetResponse(Value::Integer msgIdResponse, Value::Integer statusVal)
		: NGetResponse()
	{
		messageIdBeingRespondedTo.set(msgIdResponse);
		status.set(statusVal);
	}

	NGetResponse::NGetResponse(const Message& message)
		: NGetResponse()
	{
		copyFromMessage(message, Command::N_GET_RSP, DataSetRequirement::Optional);
	}

}

}
