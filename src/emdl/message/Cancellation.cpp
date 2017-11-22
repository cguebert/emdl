#include <emdl/message/Cancellation.h>
#include <emdl/registry.h>

namespace emdl
{
	namespace message
	{
		Cancellation::Cancellation()
			: messageIdBeingRespondedTo(initField(registry::MessageIDBeingRespondedTo))
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
