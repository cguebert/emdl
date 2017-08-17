#include <emdl/message/CFindRequest.h>
#include <emdl/registry.h>

namespace emdl
{
	namespace message
	{
		CFindRequest::CFindRequest()
			: affectedSopClass(initField(registry::AffectedSOPClassUID))
			, priority(initField(registry::Priority))
		{
			commandField.set(Command::C_FIND_RQ);
		}

		CFindRequest::CFindRequest(Value::Integer msgId, SOP_Class sopClass, Value::Integer prio, const DataSet& dataSet)
			: CFindRequest()
		{
			messageId.set(msgId);
			affectedSopClass.set(sopClass);
			priority.set(prio);

			setDataSet(dataSet);
		}

		CFindRequest::CFindRequest(const Message& message)
			: CFindRequest()
		{
			copyFromMessage(message, Command::C_FIND_RQ, DataSetRequirement::Mandatory);
		}
	}
}
