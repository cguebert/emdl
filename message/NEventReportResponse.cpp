#include <emdl/message/NEventReportResponse.h>
#include <emdl/registry.h>

namespace emdl
{
	namespace message
	{
		NEventReportResponse::NEventReportResponse()
			: affectedSopClass(initField(registry::AffectedSOPClassUID))
			, affectedSopInstance(initField(registry::AffectedSOPInstanceUID))
			, eventTypeId(initField(registry::EventTypeID))
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
