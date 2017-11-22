#include <emdl/message/NGetRequest.h>
#include <emdl/registry.h>

namespace emdl
{
	namespace message
	{
		NGetRequest::NGetRequest()
			: requestedSopClass(initField(registry::RequestedSOPClassUID))
			, requestedSopInstance(initField(registry::RequestedSOPInstanceUID))
			, attributeIdentifierList(initField(registry::AttributeIdentifierList))
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
