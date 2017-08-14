#pragma once

#include <emdl/pdu/ObjectField.h>

namespace emdl
{
	namespace pdu
	{
		// Specialization for list of a specific type of Objects
		template <class ObjectType, class SizeType>
		class ObjectsField : public BaseField
		{
		public:
			using item_type = ObjectType;
			using value_type = std::vector<item_type>;
			using size_field_type = SizeType;

			explicit ObjectsField(const BaseField::BaseInitField& init, Field<size_field_type>& sizeField, int8_t sizeDelta)
				: BaseField(init)
				, m_sizeField(sizeField)
				, m_sizeDelta(sizeDelta)
			{
			}

			const value_type& get() const
			{
				return m_items;
			}

			void set(const value_type& val)
			{
				m_items = val;
				updateSize();
			}

			/// Add one item
			void add(const item_type& val)
			{
				m_items.push_back(val);

				auto len = m_sizeField.get();
				len += static_cast<size_field_type>(Object::size(*val));
				m_sizeField.set(len);
			}

			/// Add a list of items
			void add(const value_type& val)
			{
				m_items.insert(m_items.end(), val.begin(), val.end());
				updateSize();
			}

			void clear()
			{
				m_items.clear();
				updateSize();
			}

			uint32_t size() const override
			{
				uint32_t len = 0;
				for (const auto& obj : m_items)
					len += Object::size(obj);
				return len;
			}

			void read(std::istream& in) override
			{
				const auto begin = in.tellg();
				const auto size = static_cast<size_field_type>(m_sizeField.get() - m_sizeDelta);

				m_items.clear();
				while (in.tellg() - begin < size)
					m_items.emplace_back(in);
			}

			void save(std::ostream& out) const override
			{
				for (const auto& v : m_items)
					v.save(out);
			}

		protected:
			void updateSize()
			{
				size_field_type len = m_sizeDelta;
				for (const auto& v : m_items)
					len += static_cast<size_field_type>(Object::size(v));
				m_sizeField.set(len);
			}

			value_type m_items;
			Field<size_field_type>& m_sizeField;
			int8_t m_sizeDelta = 0;
		};
	}
}
