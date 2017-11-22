#pragma once

#include <emdl/pdu/items/BaseItem.h>

namespace emdl
{
	namespace pdu
	{
		/// @Maximum Length Sub-Item Structure (PS 3.8, D.1).
		class EMDL_API MaximumLength : public BaseItem
		{
		public:
			MaximumLength(uint32_t maximumLength);
			MaximumLength(std::istream& in);

			Field<uint32_t> maximumLength;

		private:
			MaximumLength();
		};
	}
}
