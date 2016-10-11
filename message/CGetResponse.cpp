#include "CGetResponse.h"

#include <odil/registry.h>

namespace emdl
{

namespace message
{

	CGetResponse::CGetResponse()
		: messageId(initField(odil::registry::MessageID))
		, affectedSopClass(initField(odil::registry::AffectedSOPClassUID))
		, numberOfRemainingSubOperations(initField(odil::registry::NumberOfRemainingSuboperations))
		, numberOfCompletedSubOperations(initField(odil::registry::NumberOfCompletedSuboperations))
		, numberOfFailedSubOperations(initField(odil::registry::NumberOfFailedSuboperations))
		, numberOfWarningSubOperations(initField(odil::registry::NumberOfWarningSuboperations))
	{
		commandField.set(Command::C_GET_RSP);
	}

	CGetResponse::CGetResponse(Value::Integer msgIdResponse, Value::Integer statusVal)
		: CGetResponse()
	{
		messageIdBeingRespondedTo.set(msgIdResponse);
		status.set(statusVal);
	}

	CGetResponse::CGetResponse(Value::Integer msgIdResponse, Value::Integer status, const SparseDataSet& dataSet)
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
