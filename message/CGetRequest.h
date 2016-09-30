#pragma once

#include "Request.h"

namespace emds
{

namespace message
{

	//! C-GET-RQ message.
	class CGetRequest : public Request
	{
	public:
		//! Create an get request with given Message ID, affected SOP class UID, priority, and data set.
		CGetRequest(Value::Integer messageId, const Value::String& affectedSopClassUid, Value::Integer priority, const SparseDataSet& dataSet);

		//! Create a C-GET-RQ from a generic Message.
		CGetRequest(const Message& message);

		MandatoryField<Value::String> affectedSopClassUid;
		MandatoryField<Value::Integer> priority;

	private:
		CGetRequest();
	};

}

}

