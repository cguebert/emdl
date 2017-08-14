#include <emdl/pdu/AAssociate.h>

#include <emdl/pdu/items/ApplicationContext.h>
#include "odil/pdu/UserInformation.h"

namespace emdl
{
	namespace pdu
	{
		AAssociate::AAssociate()
			: protocolVersion(initField("Protocol-version", 0))
			, reserved2(initField("Reserved-2", 0))
			, calledAeTitle(initField("Called-AE-title"), 16)
			, callingAeTitle(initField("Calling-AE-title"), 16)
			, reserved3(initField("Reserved-3"), 20)
			, items(initField("Variable-items"), length, 12)
		{
		}

		AAssociate::AAssociate(std::istream& in)
			: AAssociate()
		{
			read(in);
		}

		std::string AAssociate::applicationContext() const
		{
			const auto appItems = items.get<ApplicationContext>();
			if (appItems.empty())
				throw Exception("No Application Context item");
			else if (appItems.size() > 1)
				throw Exception("Several Application Context items");
			return appItems.front()->name.get();
		}

		void AAssociate::setApplicationContext(const std::string& value)
		{
			items.remove<ApplicationContext>();
			items.add(std::make_shared<ApplicationContext>(value));
		}

		UserInformationSPtr AAssociate::userInformation() const
		{
			const auto userItems = items.get<UserInformation>();
			if (userItems.empty())
				throw Exception("No User Information item");
			else if (userItems.size() > 1)
				throw Exception("Several User Information items");
			return userItems.front();
		}

		void AAssociate::setUserInformation(const UserInformationSPtr& value)
		{
			items.remove<UserInformation>();
			items.add(value);
		}
	}
}
