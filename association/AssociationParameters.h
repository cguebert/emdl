#pragma once

#include <emdl/association/PresentationContext.h>
#include <emdl/association/UserIdentity.h>

#include <emdl/pdu/AAssociateAC.h>
#include <emdl/pdu/AAssociateRQ.h>
#include <emdl/pdu/subitems/SOPClassCommonExtendedNegotiation.h>
#include <emdl/pdu/subitems/SOPClassExtendedNegotiation.h>

namespace emdl
{
	extern EMDL_API std::string defaultImplementationClassUid; /// Implementation class UID used during negociation if another is not set
	extern EMDL_API std::string defaultImplementationVersionName; /// Implementation version name used during negociation if another is not set

	/// Encapsulate association parameters
	class EMDL_API AssociationParameters
	{
	public:
		AssociationParameters() = default;
		AssociationParameters(const pdu::AAssociateRQ& pdu);
		AssociationParameters(const pdu::AAssociateAC& pdu, const AssociationParameters& request);

		std::string calledAeTitle;
		std::string callingAeTitle;
		std::string implementationClassUid;
		std::string implementationVersionName;
		std::vector<PresentationContext> presentationContexts;
		UserIdentity userIdentity;
		uint32_t maximumLength = 16384;
		uint16_t maximumNumberOperationsInvoked = 1;
		uint16_t maximumNumberOperationsPerformed = 1;
		std::vector<std::shared_ptr<pdu::SOPClassExtendedNegotiation>> sopClassExtendedNegotiation;
		std::vector<std::shared_ptr<pdu::SOPClassCommonExtendedNegotiation>> sopClassCommonExtendedNegotiation;
	};

	std::shared_ptr<pdu::AAssociateRQ> toAAssociateRQ(const AssociationParameters& params); /// Create an A-ASSOCIATE-RQ PDU.
	std::shared_ptr<pdu::AAssociateAC> toAAssociateAC(const AssociationParameters& params); /// Create an A-ASSOCIATE-AC PDU.
}
