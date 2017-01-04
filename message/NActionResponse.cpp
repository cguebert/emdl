#include "NActionResponse.h"

#include <odil/registry.h>

namespace emdl
{

namespace message
{

	NActionResponse::NActionResponse()
		: affectedSopClass(initField(odil::registry::AffectedSOPClassUID))
		, affectedSopInstance(initField(odil::registry::AffectedSOPInstanceUID))
		, actionTypeId(initField(odil::registry::ActionTypeID))
	{
		commandField.set(Command::N_ACTION_RSP);
	}

	NActionResponse::NActionResponse(Value::Integer msgIdResponse, Value::Integer statusVal)
		: NActionResponse()
	{
		messageIdBeingRespondedTo.set(msgIdResponse);
		status.set(statusVal);
	}

	NActionResponse::NActionResponse(const Message& message)
		: NActionResponse()
	{
		copyFromMessage(message, Command::N_ACTION_RSP, DataSetRequirement::Optional);
	}

}

}
