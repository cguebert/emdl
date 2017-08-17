#include <emdl/message/CFindResponse.h>
#include <emdl/registry.h>

namespace emdl
{
	namespace message
	{
		CFindResponse::CFindResponse()
			: messageId(initField(registry::MessageID))
			, affectedSopClass(initField(registry::AffectedSOPClassUID))
		{
			commandField.set(Command::C_FIND_RSP);
		}

		CFindResponse::CFindResponse(Value::Integer msgIdResponse, Value::Integer statusVal)
			: CFindResponse()
		{
			messageIdBeingRespondedTo.set(msgIdResponse);
			status.set(statusVal);
		}

		CFindResponse::CFindResponse(Value::Integer msgIdResponse, Value::Integer statusVal, const DataSet& dataset)
			: CFindResponse(msgIdResponse, statusVal)
		{
			setDataSet(dataset);
		}

		CFindResponse::CFindResponse(const Message& message)
			: CFindResponse()
		{
			copyFromMessage(message, Command::C_FIND_RSP, DataSetRequirement::Optional);
		}
	}
}
