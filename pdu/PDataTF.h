#pragma once

#include <emdl/pdu/BasePDU.h>
#include <emdl/pdu/items/ObjectsField.h>
#include <emdl/pdu/items/PresentationDataValue.h>

namespace emdl
{
	namespace pdu
	{
		/// P-DATA-TF PDU, cf. PS 3.8, 9.3.5.
		class EMDL_API PDataTF : public BasePDU
		{
		public:
			PDataTF(const std::vector<PresentationDataValue>& pdvItems);
			PDataTF(std::istream& in);

			ObjectsField<PresentationDataValue, uint32_t> pdvItems;

		private:
			PDataTF();
		};
	}
}
