#include <emdl/association/AssociationAcceptor.h>

namespace emdl
{
	AssociationParameters defaultAssociationAcceptor(AssociationParameters const& input)
	{
		AssociationParameters output;

		output.calledAeTitle = input.calledAeTitle;
		output.callingAeTitle = input.callingAeTitle;

		output.presentationContexts = input.presentationContexts;
		for (auto& context : output.presentationContexts)
		{
			context.transferSyntaxes = {context.transferSyntaxes.front()};
			context.result = PresentationContext::Result::Acceptance;
		}

		output.maximumLength = input.maximumLength;

		return output;
	}

	AssociationRejected::AssociationRejected(uint8_t result, uint8_t source, uint8_t reason,
											 const std::string& message)
		: Exception(message)
		, result(result)
		, source(source)
		, reason(reason)
	{
	}
}
