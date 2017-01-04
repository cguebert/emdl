#include "NEventReportRequest.h"

#include <odil/registry.h>

namespace emdl
{

namespace message
{

	NEventReportRequest::NEventReportRequest()
		: affectedSopClass(initField(odil::registry::AffectedSOPClassUID))
		, affectedSopInstance(initField(odil::registry::AffectedSOPInstanceUID))
		, eventTypeId(initField(odil::registry::EventTypeID))
	{
		commandField.set(Command::N_EVENT_REPORT_RQ);
	}

	NEventReportRequest::NEventReportRequest(Value::Integer msgId, SOP_Class sopClass, SOP_Class sopInstance, Value::Integer typeId)
		: NEventReportRequest()
	{
		messageId.set(msgId);
		affectedSopClass.set(sopClass);
		affectedSopInstance.set(sopInstance);
		eventTypeId.set(typeId);
	}

	NEventReportRequest::NEventReportRequest(const Message& message)
		: NEventReportRequest()
	{
		copyFromMessage(message, Command::N_EVENT_REPORT_RQ, DataSetRequirement::Optional);
	}

}

}
