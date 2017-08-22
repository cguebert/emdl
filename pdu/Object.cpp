#include <emdl/pdu/Object.h>
#include <emdl/Exception.h>

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
			auto it = m_fields.begin(), itEnd = m_fields.end();
			std::advance(it, 3); // Do not count the first 3 fields: type, reserved and length.
			for (; it != itEnd; ++it)
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
			for (auto& field : m_fields)
				field->read(in);
		}

		void Object::save(std::ostream& out) const
		{
			for (const auto& field : m_fields)
				field->save(out);
		}

		BaseField::BaseInitField Object::initField(const char* name)
		{
			return BaseField::BaseInitField(name, *this);
		}

		void Object::addField(BaseField& field)
		{
			m_fields.push_back(&field);
		}

		std::ostream& operator<<(std::ostream& stream, const Object& object)
		{
			object.save(stream);
			return stream;
		}
	}
}
