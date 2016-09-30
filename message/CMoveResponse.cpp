#include "CMoveResponse.h"

#include <odil/registry.h>

namespace emds
{

namespace message
{

	CMoveResponse::CMoveResponse()
		: messageId(initField(odil::registry::MessageID))
		, affectedSopClassUid(initField(odil::registry::AffectedSOPClassUID))
		, numberOfRemainingSubOperations(initField(odil::registry::NumberOfRemainingSuboperations))
		, numberOfCompletedSubOperations(initField(odil::registry::NumberOfCompletedSuboperations))
		, numberOfFailedSubOperations(initField(odil::registry::NumberOfFailedSuboperations))
		, numberOfWarningSubOperations(initField(odil::registry::NumberOfWarningSuboperations))
	{
		commandField.set(Command::C_MOVE_RSP);
	}

	CMoveResponse::CMoveResponse(Value::Integer msgIdResponse, Value::Integer statusVal)
		: CMoveResponse()
	{
		messageIdBeingRespondedTo.set(msgIdResponse);
		status.set(statusVal);
	}

	CMoveResponse::CMoveResponse(Value::Integer msgIdResponse, Value::Integer status, const SparseDataSet& dataSet)
		: CMoveResponse(msgIdResponse, status)
	{
		copyDataSet(dataSet);
	}

	CMoveResponse::CMoveResponse(const Message& message)
		: CMoveResponse()
	{
		copyFromMessage(message, Command::C_MOVE_RSP, DataSetRequirement::Optional);
	}

}

}