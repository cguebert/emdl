#pragma once

#include "Request.h"

namespace emds
{

namespace message
{

	//! C-STORE-RQ message.
	class CStoreRequest: public Request
	{
	public:
		//! Create an store request with given Message ID, affected SOP class UID, priority, and data set.
		CStoreRequest(Value::Integer messageId, const Value::String& affectedSopClassUid,
			const Value::String& affectedSopInstanceUid,
			Value::Integer priority, const SparseDataSet& dataSet,
			const Value::String& moveOriginatorAet = "",
			Value::Integer moveOriginatorMessageId = -1);

		//! Create a C-STORE-RQ from a generic Message.
		CStoreRequest(const Message& message);

		MandatoryField<Value::String> affectedSopClassUid;
		MandatoryField<Value::String> affectedSopInstanceUid;
		MandatoryField<Value::Integer> priority;

		OptionalField<Value::String> moveOriginatorAet;
		OptionalField<Value::Integer> moveOriginatorMessageId;

	private:
		CStoreRequest();
	};

}

}

