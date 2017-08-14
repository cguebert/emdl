#include <emdl/pdu/items/ApplicationContext.h>
#include <emdl/pdu/items/ItemFactory.h>

namespace emdl
{
	namespace pdu
	{
		ApplicationContext::ApplicationContext()
			: name(initField("Application-context-name"))
		{
			itemType.set(ItemType::ApplicationContext);
		}

		ApplicationContext::ApplicationContext(const std::string& n)
			: ApplicationContext()
		{
			name.set(n);
		}

		ApplicationContext::ApplicationContext(std::istream& in)
			: ApplicationContext()
		{
			read(in);
		}

		RegisterItem<ApplicationContext> applicationContextItem{ItemType::ApplicationContext};
	}
}
