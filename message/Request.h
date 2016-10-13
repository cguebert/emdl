#pragma once

#include <emdl/message/Message.h>

namespace emdl
{

namespace message
{

//! Base class for all DIMSE request messages.
class EMDL_API Request: public Message
{
public:
	//! Create a request with given Message ID.
	Request(Value::Integer message_id);

	//! Create a request from the Message ID stored in the message command set.
	Request(const Message& message);
	
	MandatoryField<Value::Integer> messageId;

protected:
	Request();
};

}

}
