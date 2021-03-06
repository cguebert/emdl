#pragma once

#include <emdl/message/Request.h>

namespace emdl
{
	namespace message
	{
		//! C-FIND-RQ message.
		class EMDL_API CFindRequest : public Request
		{
		public:
			//! Create a find request with given Message ID, affected SOP class, priority, and data set.
			CFindRequest(Value::Integer messageId, SOP_Class affectedSopClass, Value::Integer priority, const DataSet& dataSet);

			//! Create a C-FIND-RQ from a generic Message.
			CFindRequest(const Message& message);

			MandatoryField<SOP_Class> affectedSopClass;
			MandatoryField<Value::Integer> priority;

		private:
			CFindRequest();
		};
	}
}
