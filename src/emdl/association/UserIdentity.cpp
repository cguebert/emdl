#include <emdl/association/UserIdentity.h>

namespace emdl
{
	UserIdentity::UserIdentity(Type type, const std::string& primaryField, const std::string& secondaryField, bool positiveResponseRequested)
		: type(type)
		, primaryField(primaryField)
		, secondaryField(secondaryField)
		, positiveResponseRequested(positiveResponseRequested)
	{
	}
}
