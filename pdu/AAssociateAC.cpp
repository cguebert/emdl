#include <emdl/pdu/AAssociateAC.h>

namespace emdl
{
	namespace pdu
	{
		AAssociateAC::AAssociateAC()
		{
			pduType.set(PDUType::A_ASSOCIATE_AC);
		}

		AAssociateAC::AAssociateAC(std::istream& in)
			: AAssociateAC()
		{
			read(in);
		}

		std::vector<PresentationContextACSPtr> AAssociateAC::presentationContexts() const
		{
			return items.get<PresentationContextAC>();
		}

		void AAssociateAC::setPresentationContexts(const std::vector<PresentationContextACSPtr>& value)
		{
			items.remove<PresentationContextAC>();
			items.add(value);
		}
	}
}
