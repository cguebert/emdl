#include <emdl/pdu/AAssociate.h>

namespace emdl
{
	namespace pdu
	{
		AAssociate::AAssociate()
			: protocolVersion(initField("Protocol-version", 0))
			, reserved2(initField("Reserved-2", 0))
			, calledAeTitle(initField("Called-AE-title"), 16)
			, callingAeTitle(initField("Calling-AE-title"), 16)
			, reserved3(initField("Reserved-3", std::string(32, '\0')), 32)
			, items(initField("Variable-items"), length, 68)
		{
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

		std::shared_ptr<UserInformation> AAssociate::userInformation() const
		{
			const auto userItems = items.get<UserInformation>();
			if (userItems.empty())
				throw Exception("No User Information item");
			else if (userItems.size() > 1)
				throw Exception("Several User Information items");
			return userItems.front();
		}

		void AAssociate::setUserInformation(const std::shared_ptr<UserInformation>& value)
		{
			items.remove<UserInformation>();
			items.add(value);
		}
	}
}
