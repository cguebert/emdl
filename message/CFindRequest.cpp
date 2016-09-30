#include "CFindRequest.h"

#include <odil/registry.h>

namespace emds
{

namespace message
{

	CFindRequest::CFindRequest()
		: affectedSopClassUid(initField(odil::registry::AffectedSOPClassUID))
		, priority(initField(odil::registry::Priority))
	{
		commandField.set(Command::C_FIND_RQ);
	}

	CFindRequest::CFindRequest(Value::Integer msgId, const Value::String& sopClass, Value::Integer prio, const SparseDataSet& dataSet)
		: CFindRequest()
	{
		messageId.set(msgId);
		affectedSopClassUid.set(sopClass);
		priority.set(prio);

		copyDataSet(dataSet);
	}

	CFindRequest::CFindRequest(const Message& message)
		: CFindRequest()
	{
		copyFromMessage(message, Command::C_FIND_RQ, DataSetRequirement::Mandatory);
	}

}

}
