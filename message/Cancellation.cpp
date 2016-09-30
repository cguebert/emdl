#include "Cancellation.h"

#include <odil/registry.h>

namespace emds
{

namespace message
{

	Cancellation::Cancellation()
		: messageIdBeingRespondedTo(initField(odil::registry::MessageIDBeingRespondedTo))
	{
		commandField.set(Command::C_CANCEL_RQ);
	}

	Cancellation::Cancellation(uint16_t id)
		: Cancellation()
	{
		messageIdBeingRespondedTo.set(id);
	}

	Cancellation::Cancellation(const Message& message)
		: Cancellation()
	{
		copyFromMessage(message, Command::C_CANCEL_RQ, DataSetRequirement::Ignore);
	}

}

}
