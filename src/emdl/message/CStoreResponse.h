#pragma once

#include <emdl/message/Response.h>

namespace emdl
{
	namespace message
	{
		//! C-STORE-RSP message.
		class EMDL_API CStoreResponse : public Response
		{
		public:
			//! C-STORE status codes, PS 3.4, B.2.3
			enum Status
			{
				// Failure
				RefusedOutOfResources = 0xA700,
				ErrorDataSetDoesNotMatchSOPClass = 0xA900,
				ErrorCannotUnderstand = 0xC000,
				// Warning
				CoercionOfDataElements = 0xB000,
				DataSetDoesNotMatchSOPClass = 0xB007,
				ElementsDiscarded = 0xB006
			};

			//! Create a store response with given Message ID, and status.
			CStoreResponse(Value::Integer messageIdBeingRespondedTo, Value::Integer status);

			//! Create a C-STORE-RSP from a generic Message.
			CStoreResponse(const Message& message);

			OptionalField<Value::Integer> messageId;
			OptionalField<SOP_Class> affectedSopClass;
			OptionalField<Value::String> affectedSopInstanceUid;

		private:
			CStoreResponse();
		};
	}
}
