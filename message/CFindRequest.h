#pragma once

#include "Request.h"

namespace emds
{

namespace message
{

	//! C-FIND-RQ message.
	class CFindRequest : public Request
	{
	public:
		//! Create an find request with given Message ID, affected SOP class UID, priority, and data set.
		CFindRequest(Value::Integer messageId, const Value::String& affectedSopClassUid, Value::Integer priority, const SparseDataSet& dataSet);

		//! Create a C-FIND-RQ from a generic Message.
		CFindRequest(const Message& message);
	
		MandatoryField<Value::String> affectedSopClassUid;
		MandatoryField<Value::Integer> priority;

	private:
		CFindRequest();
	};

}

}
