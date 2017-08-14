#pragma once

#include <emdl/pdu/BasePDU.h>
#include <emdl/pdu/items/ItemsField.h>
#include <emdl/pdu/items/PresentationDataValue.h>

namespace emdl
{
	namespace pdu
	{
		using PresentationDataValueSPtr = std::shared_ptr<PresentationDataValue>;

		/// P-DATA-TF PDU, cf. PS 3.8, 9.3.5.
		class EMDL_API PDataTF : public BasePDU
		{
		public:
			PDataTF(const std::vector<PresentationDataValueSPtr>& pdvItems);
			PDataTF(std::istream& in);

			const std::vector<PresentationDataValueSPtr> pdvItems() const;

		private:
			PDataTF();

			ItemsField<uint32_t> items;
		};
	}
}
