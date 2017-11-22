#pragma once

#include <emdl/message/Request.h>

namespace emdl
{
	namespace message
	{
		//! N-SET-RQ message.
		class EMDL_API NSetRequest : public Request
		{
		public:
			//! Create a set request with given Message ID, requested SOP class and requested SOP instance.
			NSetRequest(Value::Integer messageId, SOP_Class requestedSopClass, Value::String requestedSopInstance, const DataSet& dataSet);

			//! Create a N-SET-RQ from a generic Message.
			NSetRequest(const Message& message);

			MandatoryField<SOP_Class> requestedSopClass;
			MandatoryField<Value::String> requestedSopInstance;

		private:
			NSetRequest();
		};
	}
}
