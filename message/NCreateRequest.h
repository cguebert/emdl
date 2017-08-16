#pragma once

#include <emdl/message/Request.h>

namespace emdl
{
	namespace message
	{
		//! N-CREATE-RQ message.
		class EMDL_API NCreateRequest : public Request
		{
		public:
			//! Create a create request with given Message ID and affected SOP class.
			NCreateRequest(Value::Integer messageId, SOP_Class affectedSopClass);

			//! Create a N-CREATE-RQ from a generic Message.
			NCreateRequest(const Message& message);

			MandatoryField<SOP_Class> affectedSopClass;
			OptionalField<Value::String> affectedSopInstance;

		private:
			NCreateRequest();
		};
	}
}
