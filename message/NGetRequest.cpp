#include "NGetRequest.h"

#include <odil/registry.h>

namespace emdl
{

namespace message
{

	NGetRequest::NGetRequest()
		: requestedSopClass(initField(odil::registry::RequestedSOPClassUID))
		, requestedSopInstance(initField(odil::registry::RequestedSOPInstanceUID))
		, attributeIdentifierList(initField(odil::registry::AttributeIdentifierList))
	{
		commandField.set(Command::N_GET_RQ);
	}

	NGetRequest::NGetRequest(Value::Integer msgId, SOP_Class sopClass, Value::String sopInstance)
		: NGetRequest()
	{
		messageId.set(msgId);
		requestedSopClass.set(sopClass);
		requestedSopInstance.set(sopInstance);
	}

	NGetRequest::NGetRequest(const Message& message)
		: NGetRequest()
	{
		copyFromMessage(message, Command::N_GET_RQ, DataSetRequirement::Absent);
	}

}

}
