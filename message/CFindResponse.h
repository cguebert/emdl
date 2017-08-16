#pragma once

#include <emdl/message/Response.h>

namespace emdl
{
	namespace message
	{
		//! C-FIND-RSP message.
		class EMDL_API CFindResponse : public Response
		{
		public:
			//! C-FIND status codes, PS 3.4, C.4.1.1.4
			enum Status
			{
				// Failure
				RefusedOutOfResources = 0xA700,
				IdentifierDoesNotMatchSOPClass = 0xA900,
				UnableToProcess = 0xC000,
				// Pending
				PendingWarningOptionalKeysNotSupported = 0xFF01,
			};

			//! Create a find response with given Message ID, and status.
			CFindResponse(Value::Integer messageIdBeingRespondedTo, Value::Integer status);

			//! Create a find response with given Message ID, status, and data set.
			CFindResponse(Value::Integer messageIdBeingRespondedTo, Value::Integer status, const SparseDataSet& dataSet);

			//! Create a C-FIND-RSP from a generic Message.
			CFindResponse(const Message& message);

			OptionalField<Value::Integer> messageId;
			OptionalField<SOP_Class> affectedSopClass;

		private:
			CFindResponse();
		};
	}
}
