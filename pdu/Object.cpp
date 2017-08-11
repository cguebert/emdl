#include <emdl/pdu/Object.h>
#include <emdl/Exception.h>

#include <boost/container/flat_map.hpp>
#include <odil/registry.h>

namespace reg = odil::registry;

namespace emdl
{
	namespace pdu
	{
		Object::~Object() = default;

		const Object::Fields& Object::fields() const
		{
			return m_fields;
		}

		uint32_t Object::size() const
		{
			uint32_t len = 0;
			auto it = m_fields.begin();
			std::advance(it, 3); // Do not count the first 3 fields: type, reserved and length.
			for (; it != m_fields.end(); ++it)
				len += (*it)->size();
			return len;
		}

		uint32_t Object::size(const Object& object)
		{
			uint32_t len = 0;
			for (const auto field : object.fields())
				len += field->size();
			return len;
		}

		void Object::read(std::istream& in)
		{
		}

		void Object::save(std::ostream& out) const
		{
		}

		BaseField::BaseInitField Object::initField(const char* name)
		{
			return BaseField::BaseInitField(name, *this);
		}

		void Object::addField(BaseField& field)
		{
			m_fields.push_back(&field);
		}

		class Test : public Object
		{
		public:
			Test()
				: text(initField("text"))
				, objects(initField("objects"))
			{
				const auto len = size();
				const auto s = objects.size();
			}

			Field<std::string> text;
			Field<std::vector<Object>> objects;
		};
	}
}
