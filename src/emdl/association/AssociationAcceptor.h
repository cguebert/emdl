#pragma once

#include <emdl/association/AssociationParameters.h>
#include <emdl/Exception.h>

#include <functional>

namespace emdl
{
	/**
	 * @brief Callback to check whether the association request is acceptable.
	 *
	 * The callback shall return the parameters for the acceptation or throw an
	 * AssociationRejected exception if the association is not acceptable.
	 */
	using AssociationAcceptor = std::function<AssociationParameters(const AssociationParameters&)>;

	/**
	 * @brief Default association acceptor.
	 *
	 * Accepts the first transfer syntax and the roles of each presentation context,
	 * does not check identity, keeps maximum length.
	 */
	EMDL_API AssociationParameters defaultAssociationAcceptor(const AssociationParameters& input);

	/// Exception reported when an incoming association is rejected.
	struct EMDL_API AssociationRejected : public Exception
	{
	public:
		AssociationRejected(uint8_t result, uint8_t source, uint8_t reason,
							const std::string& message = "Association rejected");

		uint8_t result, source, reason;
	};
}
