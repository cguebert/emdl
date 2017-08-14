#pragma once

#include <emdl/pdu/AAssociate.h>
#include <emdl/pdu/items/PresentationContextRQ.h>

namespace emdl
{
	namespace pdu
	{
		using PresentationContextRQSPtr = std::shared_ptr<PresentationContextRQ>;

		/// A-ASSOCIATE-RQ, cf. PS 3.8, 9.3.2
		class EMDL_API AAssociateRQ : public AAssociate
		{
		public:
			AAssociateRQ();
			AAssociateRQ(std::istream& in);

			std::vector<PresentationContextRQSPtr> presentationContexts() const;
			void setPresentationContexts(const std::vector<PresentationContextRQSPtr>& value);
		};
	}
}
