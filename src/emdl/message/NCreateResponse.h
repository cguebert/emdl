#pragma once

#include <emdl/message/Response.h>

namespace emdl
{
	namespace message
	{
		//! N-CREATE-RSP message.
		class EMDL_API NCreateResponse : public Response
		{
		public:
			//! Create a create response with given Message ID and status.
			NCreateResponse(Value::Integer messageIdBeingRespondedTo, Value::Integer status);

			//! Create a N-CREATE-RSP from a generic Message.
			NCreateResponse(const Message& message);

			OptionalField<SOP_Class> affectedSopClass;
			OptionalField<Value::String> affectedSopInstance;

		private:
			NCreateResponse();
		};
	}
}
