#include <emdl/message/NDeleteResponse.h>
#include <emdl/registry.h>

namespace emdl
{
	namespace message
	{
		NDeleteResponse::NDeleteResponse()
			: affectedSopClass(initField(registry::AffectedSOPClassUID))
			, affectedSopInstance(initField(registry::AffectedSOPInstanceUID))
		{
			commandField.set(Command::N_DELETE_RSP);
		}

		NDeleteResponse::NDeleteResponse(Value::Integer msgIdResponse, Value::Integer statusVal)
			: NDeleteResponse()
		{
			messageIdBeingRespondedTo.set(msgIdResponse);
			status.set(statusVal);
		}

		NDeleteResponse::NDeleteResponse(const Message& message)
			: NDeleteResponse()
		{
			copyFromMessage(message, Command::N_DELETE_RSP, DataSetRequirement::Absent);
		}
	}
}
