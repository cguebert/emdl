#include <emdl/association/PresentationContext.h>

namespace emdl
{
	PresentationContext::PresentationContext(uint8_t id,
											 SOP_Class abstractSyntax,
											 const std::vector<TransferSyntax>& transferSyntaxes,
											 bool scuRoleSupport, bool scpRoleSupport, bool roleSelectionPresent,
											 Result result)
		: id(id)
		, abstractSyntax(abstractSyntax)
		, transferSyntaxes(transferSyntaxes)
		, scuRoleSupport(scuRoleSupport)
		, scpRoleSupport(scpRoleSupport)
		, roleSelectionPresent(roleSelectionPresent)
		, result(result)
	{
	}
}
