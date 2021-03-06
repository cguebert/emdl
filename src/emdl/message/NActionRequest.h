#pragma once

#include <emdl/message/Request.h>

namespace emdl
{
	namespace message
	{
		//! N-ACTION-RQ message.
		class EMDL_API NActionRequest : public Request
		{
		public:
			//! Create an action request with given Message ID, requested SOP class, requested SOP instance and action type ID.
			NActionRequest(Value::Integer messageId, SOP_Class requestedSopClass, Value::String requestedSopInstance, Value::Integer actionTypeId);

			//! Create a N-ACTION-RQ from a generic Message.
			NActionRequest(const Message& message);

			MandatoryField<SOP_Class> requestedSopClass;
			MandatoryField<Value::String> requestedSopInstance;
			MandatoryField<Value::Integer> actionTypeId;

		private:
			NActionRequest();
		};
	}
}
