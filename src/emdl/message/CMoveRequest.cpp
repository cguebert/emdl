#include <emdl/message/CMoveRequest.h>
#include <emdl/registry.h>

namespace emdl
{
	namespace message
	{
		CMoveRequest::CMoveRequest()
			: affectedSopClass(initField(registry::AffectedSOPClassUID))
			, priority(initField(registry::Priority))
			, moveDestination(initField(registry::MoveDestination))
		{
			commandField.set(Command::C_MOVE_RQ);
		}

		CMoveRequest::CMoveRequest(Value::Integer msgId, SOP_Class sopClass, Value::Integer prio, const Value::String& dest, const DataSet& dataSet)
			: CMoveRequest()
		{
			messageId.set(msgId);
			affectedSopClass.set(sopClass);
			priority.set(prio);
			moveDestination.set(dest);

			setDataSet(dataSet);
		}

		CMoveRequest::CMoveRequest(const Message& message)
			: CMoveRequest()
		{
			copyFromMessage(message, Command::C_MOVE_RQ, DataSetRequirement::Mandatory);
		}
	}
}
