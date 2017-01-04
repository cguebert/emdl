#include "NCreateRequest.h"

#include <odil/registry.h>

namespace emdl
{

namespace message
{

	NCreateRequest::NCreateRequest()
		: affectedSopClass(initField(odil::registry::AffectedSOPClassUID))
		, affectedSopInstance(initField(odil::registry::AffectedSOPInstanceUID))
	{
		commandField.set(Command::N_CREATE_RQ);
	}

	NCreateRequest::NCreateRequest(Value::Integer msgId, SOP_Class sopClass)
		: NCreateRequest()
	{
		messageId.set(msgId);
		affectedSopClass.set(sopClass);
	}

	NCreateRequest::NCreateRequest(const Message& message)
		: NCreateRequest()
	{
		copyFromMessage(message, Command::N_CREATE_RQ, DataSetRequirement::Optional);
	}

}

}
