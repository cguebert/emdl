#include "CStoreRequest.h"

#include <odil/registry.h>

namespace emdl
{

namespace message
{

	CStoreRequest::CStoreRequest()
		: affectedSopClassUid(initField(odil::registry::AffectedSOPClassUID))
		, affectedSopInstanceUid(initField(odil::registry::AffectedSOPInstanceUID))
		, priority(initField(odil::registry::Priority))
		, moveOriginatorAet(initField(odil::registry::MoveOriginatorApplicationEntityTitle))
		, moveOriginatorMessageId(initField(odil::registry::MoveOriginatorMessageID))
	{
		commandField.set(Command::C_STORE_RQ);
	}

	CStoreRequest::CStoreRequest(Value::Integer msgId, const Value::String& sopClass, const Value::String& sopInstance,
		Value::Integer prio, const SparseDataSet& dataSet, const Value::String& originAet, Value::Integer originMsgId)
		: CStoreRequest()
	{
		messageId.set(msgId);
		affectedSopClassUid.set(sopClass);
		affectedSopInstanceUid.set(sopInstance);
		priority.set(prio);

		if (!originAet.empty())
			moveOriginatorAet.set(originAet);
		if (originMsgId >= 0)
			moveOriginatorMessageId.set(originMsgId);

		setDataSet(dataSet);
	}

	CStoreRequest::CStoreRequest(const Message& message)
		: CStoreRequest()
	{
		copyFromMessage(message, Command::C_STORE_RQ, DataSetRequirement::Mandatory);
	}

}

}
