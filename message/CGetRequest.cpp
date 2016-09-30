#include "CGetRequest.h"

#include <odil/registry.h>

namespace emds
{

namespace message
{

	CGetRequest::CGetRequest()
		: affectedSopClassUid(initField(odil::registry::AffectedSOPClassUID))
		, priority(initField(odil::registry::Priority))
	{
		commandField.set(Command::C_GET_RQ);
	}

	CGetRequest::CGetRequest(Value::Integer msgId, const Value::String& sopClass, Value::Integer prio, const SparseDataSet& dataSet)
		: CGetRequest()
	{
		messageId.set(msgId);
		affectedSopClassUid.set(sopClass);
		priority.set(prio);

		copyDataSet(dataSet);
	}

	CGetRequest::CGetRequest(const Message& message)
		: CGetRequest()
	{
		copyFromMessage(message, Command::C_GET_RQ, DataSetRequirement::Mandatory);
	}

}

}
