#include <emdl/message/NEventReportRequest.h>
#include <emdl/registry.h>

namespace emdl
{
	namespace message
	{
		NEventReportRequest::NEventReportRequest()
			: affectedSopClass(initField(registry::AffectedSOPClassUID))
			, affectedSopInstance(initField(registry::AffectedSOPInstanceUID))
			, eventTypeId(initField(registry::EventTypeID))
		{
			commandField.set(Command::N_EVENT_REPORT_RQ);
		}

		NEventReportRequest::NEventReportRequest(Value::Integer msgId, SOP_Class sopClass, Value::String sopInstance, Value::Integer typeId)
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
