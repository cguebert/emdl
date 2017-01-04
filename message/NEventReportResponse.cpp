#include "NEventReportResponse.h"

#include <odil/registry.h>

namespace emdl
{

namespace message
{

	NEventReportResponse::NEventReportResponse()
		: affectedSopClass(initField(odil::registry::AffectedSOPClassUID))
		, affectedSopInstance(initField(odil::registry::AffectedSOPInstanceUID))
		, eventTypeId(initField(odil::registry::EventTypeID))
	{
		commandField.set(Command::N_EVENT_REPORT_RSP);
	}

	NEventReportResponse::NEventReportResponse(Value::Integer msgIdResponse, Value::Integer statusVal)
		: NEventReportResponse()
	{
		messageIdBeingRespondedTo.set(msgIdResponse);
		status.set(statusVal);
	}

	NEventReportResponse::NEventReportResponse(const Message& message)
		: NEventReportResponse()
	{
		copyFromMessage(message, Command::N_EVENT_REPORT_RSP, DataSetRequirement::Optional);
	}

}

}
