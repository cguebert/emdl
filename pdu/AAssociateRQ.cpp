#include <emdl/pdu/AAssociateRQ.h>

namespace emdl
{
	namespace pdu
	{
		AAssociateRQ::AAssociateRQ()
		{
			pduType.set(PDUType::A_ASSOCIATE_RQ);
		}

		AAssociateRQ::AAssociateRQ(std::istream& in)
			: AAssociateRQ()
		{
			read(in);
		}

		std::vector<PresentationContextRQSPtr> AAssociateRQ::presentationContexts() const
		{
			return items.get<PresentationContextRQ>();
		}

		void AAssociateRQ::setPresentationContexts(const std::vector<PresentationContextRQSPtr>& value)
		{
			items.remove<PresentationContextRQ>();
			items.add(value);
		}
	}
}
