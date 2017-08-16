#pragma once

#include <emdl/emdl_api.h>

#include <string>

namespace emdl
{
	/// User Identity, cf. PS3.8 D.3.3.7
	struct EMDL_API UserIdentity
	{
		enum class Type
		{
			None = 0,
			Username = 1,
			UsernameAndPassword = 2,
			Kerberos = 3,
			SAML = 4
		};

		UserIdentity() = default;
		UserIdentity(Type type,
					 const std::string& primaryField,
					 const std::string& secondaryField,
					 bool positiveResponseRequested = false);

		Type type = Type::None;
		std::string primaryField, secondaryField;
		bool positiveResponseRequested = false;
	};
}
