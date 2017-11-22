#include <emdl/message/CGetResponse.h>
#include <emdl/registry.h>

namespace emdl
{
	namespace message
	{
		CGetResponse::CGetResponse()
			: messageId(initField(registry::MessageID))
			, affectedSopClass(initField(registry::AffectedSOPClassUID))
			, numberOfRemainingSubOperations(initField(registry::NumberOfRemainingSuboperations))
			, numberOfCompletedSubOperations(initField(registry::NumberOfCompletedSuboperations))
			, numberOfFailedSubOperations(initField(registry::NumberOfFailedSuboperations))
			, numberOfWarningSubOperations(initField(registry::NumberOfWarningSuboperations))
		{
			commandField.set(Command::C_GET_RSP);
		}

		CGetResponse::CGetResponse(Value::Integer msgIdResponse, Value::Integer statusVal)
			: CGetResponse()
		{
			messageIdBeingRespondedTo.set(msgIdResponse);
			status.set(statusVal);
		}

		CGetResponse::CGetResponse(Value::Integer msgIdResponse, Value::Integer status, const DataSet& dataSet)
			: CGetResponse(msgIdResponse, status)
		{
			setDataSet(dataSet);
		}

		CGetResponse::CGetResponse(const Message& message)
			: CGetResponse()
		{
			copyFromMessage(message, Command::C_GET_RSP, DataSetRequirement::Optional);
		}
	}
}
