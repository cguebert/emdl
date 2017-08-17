#include <emdl/message/CMoveResponse.h>
#include <emdl/registry.h>

namespace emdl
{
	namespace message
	{
		CMoveResponse::CMoveResponse()
			: messageId(initField(registry::MessageID))
			, affectedSopClass(initField(registry::AffectedSOPClassUID))
			, numberOfRemainingSubOperations(initField(registry::NumberOfRemainingSuboperations))
			, numberOfCompletedSubOperations(initField(registry::NumberOfCompletedSuboperations))
			, numberOfFailedSubOperations(initField(registry::NumberOfFailedSuboperations))
			, numberOfWarningSubOperations(initField(registry::NumberOfWarningSuboperations))
		{
			commandField.set(Command::C_MOVE_RSP);
		}

		CMoveResponse::CMoveResponse(Value::Integer msgIdResponse, Value::Integer statusVal)
			: CMoveResponse()
		{
			messageIdBeingRespondedTo.set(msgIdResponse);
			status.set(statusVal);
		}

		CMoveResponse::CMoveResponse(Value::Integer msgIdResponse, Value::Integer status, const DataSet& dataSet)
			: CMoveResponse(msgIdResponse, status)
		{
			setDataSet(dataSet);
		}

		CMoveResponse::CMoveResponse(const Message& message)
			: CMoveResponse()
		{
			copyFromMessage(message, Command::C_MOVE_RSP, DataSetRequirement::Optional);
		}
	}
}
