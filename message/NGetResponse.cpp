#include <emdl/message/NGetResponse.h>
#include <emdl/registry.h>

namespace emdl
{
	namespace message
	{
		NGetResponse::NGetResponse()
			: affectedSopClass(initField(registry::AffectedSOPClassUID))
			, affectedSopInstance(initField(registry::AffectedSOPInstanceUID))
		{
			commandField.set(Command::N_GET_RSP);
		}

		NGetResponse::NGetResponse(Value::Integer msgIdResponse, Value::Integer statusVal)
			: NGetResponse()
		{
			messageIdBeingRespondedTo.set(msgIdResponse);
			status.set(statusVal);
		}

		NGetResponse::NGetResponse(const Message& message)
			: NGetResponse()
		{
			copyFromMessage(message, Command::N_GET_RSP, DataSetRequirement::Optional);
		}
	}
}
