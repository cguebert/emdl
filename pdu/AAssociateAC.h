#pragma once

#include <emdl/pdu/AAssociate.h>
#include <emdl/pdu/items/PresentationContextAC.h>

namespace emdl
{
	namespace pdu
	{
		using PresentationContextACSPtr = std::shared_ptr<PresentationContextAC>;

		/// A-ASSOCIATE-AC PDU, cf. PS 3.8, 9.3.3.
		class EMDL_API AAssociateAC : public AAssociate
		{
		public:
			AAssociateAC();
			AAssociateAC(std::istream& in);

			std::vector<PresentationContextACSPtr> presentationContexts() const;
			void setPresentationContexts(const std::vector<PresentationContextACSPtr>& contexts);
		};
	}
}
