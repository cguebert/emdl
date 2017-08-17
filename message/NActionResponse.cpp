#include <emdl/message/NActionResponse.h>
#include <emdl/registry.h>

namespace emdl
{
	namespace message
	{
		NActionResponse::NActionResponse()
			: affectedSopClass(initField(registry::AffectedSOPClassUID))
			, affectedSopInstance(initField(registry::AffectedSOPInstanceUID))
			, actionTypeId(initField(registry::ActionTypeID))
		{
			commandField.set(Command::N_ACTION_RSP);
		}

		NActionResponse::NActionResponse(Value::Integer msgIdResponse, Value::Integer statusVal)
			: NActionResponse()
		{
			messageIdBeingRespondedTo.set(msgIdResponse);
			status.set(statusVal);
		}

		NActionResponse::NActionResponse(const Message& message)
			: NActionResponse()
		{
			copyFromMessage(message, Command::N_ACTION_RSP, DataSetRequirement::Optional);
		}
	}
}
