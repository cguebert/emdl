#include "NDeleteResponse.h"

#include <odil/registry.h>

namespace emdl
{

namespace message
{

	NDeleteResponse::NDeleteResponse()
		: affectedSopClass(initField(odil::registry::AffectedSOPClassUID))
		, affectedSopInstance(initField(odil::registry::AffectedSOPInstanceUID))
	{
		commandField.set(Command::N_DELETE_RSP);
	}

	NDeleteResponse::NDeleteResponse(Value::Integer msgIdResponse, Value::Integer statusVal)
		: NDeleteResponse()
	{
		messageIdBeingRespondedTo.set(msgIdResponse);
		status.set(statusVal);
	}

	NDeleteResponse::NDeleteResponse(const Message& message)
		: NDeleteResponse()
	{
		copyFromMessage(message, Command::N_DELETE_RSP, DataSetRequirement::Absent);
	}

}

}
