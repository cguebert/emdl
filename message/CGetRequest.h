#pragma once

#include <emdl/message/Request.h>

namespace emdl
{
	namespace message
	{
		//! C-GET-RQ message.
		class EMDL_API CGetRequest : public Request
		{
		public:
			//! Create a get request with given Message ID, affected SOP class, priority, and data set.
			CGetRequest(Value::Integer messageId, SOP_Class affectedSopClass, Value::Integer priority, const SparseDataSet& dataSet);

			//! Create a C-GET-RQ from a generic Message.
			CGetRequest(const Message& message);

			MandatoryField<SOP_Class> affectedSopClass;
			MandatoryField<Value::Integer> priority;

		private:
			CGetRequest();
		};
	}
}
