#pragma once

#include <emdl/SOPClasses.h>
#include <emdl/TransferSyntaxes.h>

#include <string>
#include <vector>

namespace emdl
{
	/// Presentation Context, cf. PS 3.8, 9.3.2.2, PS 3.8, 9.3.3.2, PS 3.7, D.3.3.4.1 and PS 3.7 D.3.3.4.2.
	struct EMDL_API PresentationContext
	{
		enum class Result
		{
			Acceptance = 0,
			UserRejection = 1,
			NoReason = 2,
			AbstractSyntaxNotSupported = 3,
			TransferSyntaxesNotSupported = 4,
		};

		PresentationContext(uint8_t id,
							SOP_Class abstractSyntax,
							const std::vector<TransferSyntax>& transferSyntaxes,
							bool scuRoleSupport, bool scpRoleSupport, bool roleSelectionPresent = true,
							Result result = Result::NoReason);

		uint8_t id = 0;

		SOP_Class abstractSyntax;
		std::vector<TransferSyntax> transferSyntaxes;

		bool scuRoleSupport, scpRoleSupport;
		bool roleSelectionPresent;

		Result result;
	};
}
