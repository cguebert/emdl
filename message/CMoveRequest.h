#pragma once

#include "Request.h"

namespace emdl
{

namespace message
{

	//! C-MOVE-RQ message.
	class CMoveRequest : public Request
	{
	public:
		//! Create an move request with given Message ID, affected SOP class UID, priority, move destination, and data set.
		CMoveRequest(Value::Integer messageId, const Value::String& affectedSopClassUid, Value::Integer priority, const Value::String& moveDestination, const SparseDataSet& dataSet);

		//! Create a C-MOVE-RQ from a generic Message.
		CMoveRequest(const Message& message);

		MandatoryField<Value::String> affectedSopClassUid;
		MandatoryField<Value::Integer> priority;
		MandatoryField<Value::String> moveDestination;

	private:
		CMoveRequest();
	};

}

}

