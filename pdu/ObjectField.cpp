#include <emdl/pdu/Object.h>

namespace emdl
{
	namespace pdu
	{
		BaseField::~BaseField() = default;

		BaseField::BaseField(const BaseInitField& init)
			: m_name(init.name)
		{
			init.registerField(*this);
		}

		const char* BaseField::name() const
		{
			return m_name;
		}

		void BaseField::BaseInitField::registerField(BaseField& field) const
		{
			owner.addField(field);
		}
	}
}
