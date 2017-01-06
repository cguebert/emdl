#include "NDeleteRequest.h"

#include <odil/registry.h>

namespace emdl
{

namespace message
{

	NDeleteRequest::NDeleteRequest()
		: requestedSopClass(initField(odil::registry::RequestedSOPClassUID))
		, requestedSopInstance(initField(odil::registry::RequestedSOPInstanceUID))
	{
		commandField.set(Command::N_DELETE_RQ);
	}

	NDeleteRequest::NDeleteRequest(Value::Integer msgId, SOP_Class sopClass, Value::String sopInstance)
		: NDeleteRequest()
	{
		messageId.set(msgId);
		requestedSopClass.set(sopClass);
		requestedSopInstance.set(sopInstance);
	}

	NDeleteRequest::NDeleteRequest(const Message& message)
		: NDeleteRequest()
	{
		copyFromMessage(message, Command::N_DELETE_RQ, DataSetRequirement::Absent);
	}

}

}
