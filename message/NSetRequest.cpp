#include <emdl/message/NSetRequest.h>
#include <emdl/registry.h>

namespace emdl
{
	namespace message
	{
		NSetRequest::NSetRequest()
			: requestedSopClass(initField(registry::RequestedSOPClassUID))
			, requestedSopInstance(initField(registry::RequestedSOPInstanceUID))
		{
			commandField.set(Command::N_SET_RQ);
		}

		NSetRequest::NSetRequest(Value::Integer msgId, SOP_Class sopClass, Value::String sopInstance)
			: NSetRequest()
		{
			messageId.set(msgId);
			requestedSopClass.set(sopClass);
			requestedSopInstance.set(sopInstance);
		}

		NSetRequest::NSetRequest(const Message& message)
			: NSetRequest()
		{
			copyFromMessage(message, Command::N_SET_RQ, DataSetRequirement::Mandatory);
		}
	}
}
