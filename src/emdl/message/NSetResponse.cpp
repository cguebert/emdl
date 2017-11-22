#include <emdl/message/NSetResponse.h>
#include <emdl/registry.h>

namespace emdl
{
	namespace message
	{
		NSetResponse::NSetResponse()
			: affectedSopClass(initField(registry::AffectedSOPClassUID))
			, affectedSopInstance(initField(registry::AffectedSOPInstanceUID))
		{
			commandField.set(Command::N_SET_RSP);
		}

		NSetResponse::NSetResponse(Value::Integer msgIdResponse, Value::Integer statusVal)
			: NSetResponse()
		{
			messageIdBeingRespondedTo.set(msgIdResponse);
			status.set(statusVal);
		}

		NSetResponse::NSetResponse(const Message& message)
			: NSetResponse()
		{
			copyFromMessage(message, Command::N_SET_RSP, DataSetRequirement::Optional);
		}
	}
}
