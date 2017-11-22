#pragma once

#include <emdl/message/Request.h>

namespace emdl
{
	namespace message
	{
		//! C-ECHO-RQ message.
		class EMDL_API CEchoRequest : public Request
		{
		public:
			//! Create an echo request with given Message ID and affected SOP class.
			CEchoRequest(Value::Integer messageId, SOP_Class affectedSopClass);

			//! Create a C-ECHO-RQ from a generic Message.
			CEchoRequest(const Message& message);

			MandatoryField<SOP_Class> affectedSopClass;

		private:
			CEchoRequest();
		};
	}
}
