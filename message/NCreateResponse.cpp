#include "NCreateResponse.h"

#include <odil/registry.h>

namespace emdl
{

namespace message
{

	NCreateResponse::NCreateResponse()
		: affectedSopClass(initField(odil::registry::AffectedSOPClassUID))
		, affectedSopInstance(initField(odil::registry::AffectedSOPInstanceUID))
	{
		commandField.set(Command::N_CREATE_RSP);
	}

	NCreateResponse::NCreateResponse(Value::Integer msgIdResponse, Value::Integer statusVal)
		: NCreateResponse()
	{
		messageIdBeingRespondedTo.set(msgIdResponse);
		status.set(statusVal);
	}

	NCreateResponse::NCreateResponse(const Message& message)
		: NCreateResponse()
	{
		copyFromMessage(message, Command::N_CREATE_RSP, DataSetRequirement::Optional);
	}

}

}
