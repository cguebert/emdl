#pragma once

#include <emdl/pdu/items/PresentationContext.h>
#include <emdl/SOPClasses.h>
#include <emdl/TransferSyntaxes.h>

namespace emdl
{
	namespace pdu
	{
		/// Presentation Context item for a A-ASSOCIATE-RQ PDU (PS 3.8, 9.3.2.2).
		class EMDL_API PresentationContextRQ : public PresentationContext
		{
		public:
			using TransferSyntaxes = std::vector<TransferSyntax>;

			PresentationContextRQ(uint8_t id, SOP_Class abstractSyntax, const TransferSyntaxes& transferSyntaxes);
			PresentationContextRQ(std::istream& in);

			SOP_Class abstractSyntax() const;
			void setAbstractSyntax(SOP_Class abstractSyntax);

			std::vector<TransferSyntax> transferSyntaxes() const;
			void setTransferSyntaxes(const TransferSyntaxes& transferSyntaxes);

		private:
			PresentationContextRQ();
		};
	}
}
