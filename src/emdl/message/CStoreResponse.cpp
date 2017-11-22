#include <emdl/message/CStoreResponse.h>
#include <emdl/registry.h>

namespace emdl
{
	namespace message
	{
		CStoreResponse::CStoreResponse()
			: messageId(initField(registry::MessageID))
			, affectedSopClass(initField(registry::AffectedSOPClassUID))
			, affectedSopInstanceUid(initField(registry::AffectedSOPInstanceUID))
		{
			commandField.set(Command::C_STORE_RSP);
		}

		CStoreResponse::CStoreResponse(Value::Integer msgIdResponse, Value::Integer statusVal)
			: CStoreResponse()
		{
			messageIdBeingRespondedTo.set(msgIdResponse);
			status.set(statusVal);
		}

		CStoreResponse::CStoreResponse(const Message& message)
			: CStoreResponse()
		{
			copyFromMessage(message, Command::C_STORE_RSP, DataSetRequirement::Absent);
		}
	}
}
