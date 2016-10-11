#pragma once

#include "Response.h"

namespace emdl
{

namespace message
{

	//! C-GET-RSP message.
	class CGetResponse : public Response
	{
	public:
		//! C-GET status codes, PS 3.4, C.4.3.1.4
		enum Status
		{
			// Failure
			RefusedOutOfResourcesUnableToCalculateNumberOfMatches = 0xA701,
			RefusedOutOfResourcesUnableToPerformSubOperations = 0xA702,
			IdentifierDoesNotMatchSOPClass = 0xA900,
			UnableToProcess = 0xC000,
			// Warning
			SubOperationsCompleteOneOrMoreFailuresOrWarnings = 0xB000
		};

		//! Create an get response with given Message ID, and status.
		CGetResponse(Value::Integer messageIdBeingRespondedTo, Value::Integer status);

		//! Create an get response with given Message ID, status, and data set.
		CGetResponse(Value::Integer messageIdBeingRespondedTo, Value::Integer status, const SparseDataSet& dataSet);

		//! Create a C-GET-RSP from a generic Message.
		CGetResponse(const Message& message);

		OptionalField<Value::Integer> messageId;
		OptionalField<SOP_Class> affectedSopClass;

		OptionalField<Value::Integer> numberOfRemainingSubOperations;
		OptionalField<Value::Integer> numberOfCompletedSubOperations;
		OptionalField<Value::Integer> numberOfFailedSubOperations;
		OptionalField<Value::Integer> numberOfWarningSubOperations;

	private:
		CGetResponse();
	};

}

}
