#pragma once

#include <emdl/pdu/items/PresentationContext.h>
#include <emdl/TransferSyntaxes.h>

namespace emdl
{
	namespace pdu
	{
		/// Presentation Context item for a A-ASSOCIATE-AC PDU (PS 3.8, 9.3.3.2).
		class EMDL_API PresentationContextAC : public PresentationContext
		{
		public:
			PresentationContextAC(uint8_t id, TransferSyntax transferSyntax, uint8_t result);
			PresentationContextAC(std::istream& in);

			TransferSyntax transferSyntax() const;
			void setTransferSyntax(TransferSyntax ts);

		private:
			PresentationContextAC();
		};
	}
}
