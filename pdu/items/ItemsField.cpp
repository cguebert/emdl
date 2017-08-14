#include <emdl/pdu/items/ItemsField.h>
#include <emdl/pdu/Object.h>

namespace emdl
{
	namespace pdu
	{
		// TODO: remove this test
		class Test : public Object
		{
		public:
			Test()
				: len(initField("length", 0))
				, objects(initField("objects"), len)
			{
				const auto len = size();
				const auto s = objects.size();
			}

			Field<uint32_t> len;
			ItemsField<uint32_t> objects;
		};
	}
}
