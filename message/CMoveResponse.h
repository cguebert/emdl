#pragma once

#include "Response.h"

namespace emds
{

namespace message
{

	//! C-MOVE-RSP message.
	class CMoveResponse: public Response
	{
	public:
		//! C-MOVE status codes, PS 3.4, C.4.2.1.5
		enum Status
		{
			// Failure
			RefusedOutOfResourcesUnableToCalculateNumberOfMatches = 0xA701,
			RefusedOutOfResourcesUnableToPerformSubOperations = 0xA702,
			RefusedMoveDestinationUnknown = 0xA801,
			IdentifierDoesNotMatchSOPClass = 0xA900,
			UnableToProcess = 0xC000,
			// Warning
			SubOperationsCompleteOneOrMoreFailuresOrWarnings = 0xB000
		};

		//! Create an move response with given Message ID, and status.
		CMoveResponse(Value::Integer messageIdBeingRespondedTo, Value::Integer status);

		//! Create an move response with given Message ID, status, and data set.
		CMoveResponse(Value::Integer messageIdBeingRespondedTo, Value::Integer status, const SparseDataSet& dataSet);

		//! Create a C-MOVE-RSP from a generic Message.
		CMoveResponse(const Message& message);

		OptionalField<Value::Integer> messageId;
		OptionalField<Value::String> affectedSopClassUid;

		OptionalField<Value::Integer> numberOfRemainingSubOperations;
		OptionalField<Value::Integer> numberOfCompletedSubOperations;
		OptionalField<Value::Integer> numberOfFailedSubOperations;
		OptionalField<Value::Integer> numberOfWarningSubOperations;

	private:
		CMoveResponse();
	};

}

}