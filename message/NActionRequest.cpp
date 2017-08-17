#include <emdl/message/NActionRequest.h>
#include <emdl/registry.h>

namespace emdl
{
	namespace message
	{
		NActionRequest::NActionRequest()
			: requestedSopClass(initField(registry::RequestedSOPClassUID))
			, requestedSopInstance(initField(registry::RequestedSOPInstanceUID))
			, actionTypeId(initField(registry::ActionTypeID))
		{
			commandField.set(Command::N_ACTION_RQ);
		}

		NActionRequest::NActionRequest(Value::Integer msgId, SOP_Class sopClass, Value::String sopInstance, Value::Integer typeId)
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
