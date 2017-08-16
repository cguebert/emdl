#include <emdl/pdu/items/BasePresentationContext.h>

namespace emdl
{
	namespace pdu
	{
		PresentationContext::PresentationContext()
			: presentationContextId(initField("Presentation-context-id", 0))
			, reserved2(initField("Reserved-2", 0))
			, result(initField("Result", 0))
			, reserved3(initField("Reserved-3", 0))
			, subItems(initField("Sub-Items"), length, 4)
		{
		}
	}
}
