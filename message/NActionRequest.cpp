#include "NActionRequest.h"

#include <odil/registry.h>

namespace emdl
{

namespace message
{

	NActionRequest::NActionRequest()
		: requestedSopClass(initField(odil::registry::RequestedSOPClassUID))
		, requestedSopInstance(initField(odil::registry::RequestedSOPInstanceUID))
		, actionTypeId(initField(odil::registry::ActionTypeID))
	{
		commandField.set(Command::N_ACTION_RQ);
	}

	NActionRequest::NActionRequest(Value::Integer msgId, SOP_Class sopClass, SOP_Class sopInstance, Value::Integer typeId)
		: NActionRequest()
	{
		messageId.set(msgId);
		requestedSopClass.set(sopClass);
		requestedSopInstance.set(sopInstance);
		actionTypeId.set(typeId);
	}

	NActionRequest::NActionRequest(const Message& message)
		: NActionRequest()
	{
		copyFromMessage(message, Command::N_ACTION_RQ, DataSetRequirement::Optional);
	}

}

}
