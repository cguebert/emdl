#include <emdl/pdu/items/UserInformation.h>

namespace emdl
{
	namespace pdu
	{
		UserInformation::UserInformation()
			: subItems(initField("User-data"), length, 0)
		{
		}

		UserInformation::UserInformation(std::istream& in)
			: UserInformation()
		{
			read(in);
		}
	}
}
