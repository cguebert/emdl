#include <emdl/message/NDeleteRequest.h>
#include <emdl/registry.h>

namespace emdl
{
	namespace message
	{
		NDeleteRequest::NDeleteRequest()
			: requestedSopClass(initField(registry::RequestedSOPClassUID))
			, requestedSopInstance(initField(registry::RequestedSOPInstanceUID))
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
