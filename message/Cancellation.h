#pragma once

#include "Message.h"

namespace emdl
{

namespace message
{

	//! Base class for cancellation messages.
	class EMDL_API Cancellation : public Message
	{
	public:
		//! Create a response with given message id being responded to.
		Cancellation(uint16_t messageIdBeingRespondedTo);

		//! Create a response from the Message ID Being Responded To.
		Cancellation(const Message& message);

		MandatoryField<Value::Integer> messageIdBeingRespondedTo;

	private:
		Cancellation();
	};

}

}
