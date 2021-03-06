#pragma once

#include <emdl/message/Request.h>

namespace emdl
{
	namespace message
	{
		//! N-EVENT-REPORT-RQ message.
		class EMDL_API NEventReportRequest : public Request
		{
		public:
			//! Create an event report request with given Message ID, affected SOP class, affected SOP instance and event type ID.
			NEventReportRequest(Value::Integer messageId, SOP_Class affectedSopClass, Value::String affectedSopInstance, Value::Integer eventTypeId);

			//! Create a N-EVENT-REPORT-RQ from a generic Message.
			NEventReportRequest(const Message& message);

			MandatoryField<SOP_Class> affectedSopClass;
			MandatoryField<Value::String> affectedSopInstance;
			MandatoryField<Value::Integer> eventTypeId;

		private:
			NEventReportRequest();
		};
	}
}
