#pragma once

#include <emdl/message/Request.h>

namespace emdl
{
	namespace message
	{
		//! C-MOVE-RQ message.
		class EMDL_API CMoveRequest : public Request
		{
		public:
			//! Create a move request with given Message ID, affected SOP class, priority, move destination, and data set.
			CMoveRequest(Value::Integer messageId,
						 SOP_Class affectedSopClass,
						 Value::Integer priority,
						 const Value::String& moveDestination,
						 const SparseDataSet& dataSet);

			//! Create a C-MOVE-RQ from a generic Message.
			CMoveRequest(const Message& message);

			MandatoryField<SOP_Class> affectedSopClass;
			MandatoryField<Value::Integer> priority;
			MandatoryField<Value::String> moveDestination;

		private:
			CMoveRequest();
		};
	}
}
