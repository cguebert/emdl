#include "CGetRequest.h"

#include <odil/registry.h>

namespace emdl
{
	namespace message
	{
		CGetRequest::CGetRequest()
			: affectedSopClass(initField(odil::registry::AffectedSOPClassUID))
			, priority(initField(odil::registry::Priority))
		{
			commandField.set(Command::C_GET_RQ);
		}

		CGetRequest::CGetRequest(Value::Integer msgId, SOP_Class sopClass, Value::Integer prio, const DataSet& dataSet)
			: CGetRequest()
		{
			messageId.set(msgId);
			affectedSopClass.set(sopClass);
			priority.set(prio);

			setDataSet(dataSet);
		}

		CGetRequest::CGetRequest(const Message& message)
			: CGetRequest()
		{
			copyFromMessage(message, Command::C_GET_RQ, DataSetRequirement::Mandatory);
		}
	}
}
