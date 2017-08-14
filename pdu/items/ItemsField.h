#pragma once

#include <emdl/pdu/ObjectField.h>
#include <emdl/pdu/items/BaseItem.h>
#include <emdl/pdu/items/ItemFactory.h>

#include <algorithm>
#include <iterator>
#include <memory>

namespace emdl
{
	namespace pdu
	{
		// Specialization for list of Objects
		template <class S>
		class ItemsField : public BaseField
		{
		public:
			using item_type = std::shared_ptr<BaseItem>;
			using value_type = std::vector<item_type>;
			using size_field_type = S;

			explicit ItemsField(const BaseField::BaseInitField& init, Field<size_field_type>& sizeField, int8_t sizeDelta)
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

			/// Add a list of objects (derived from BaseItem)
			template <class U>
			std::enable_if_t<std::is_base_of_v<BaseItem, U>> add(const std::vector<std::shared_ptr<U>>& val)
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
					len += Object::size(*obj);
				return len;
			}

			void read(std::istream& in) override
			{
				const auto begin = in.tellg();
				const auto size = static_cast<size_field_type>(m_sizeField.get() - m_sizeDelta);

				m_items.clear();
				while (in.tellg() - begin < size)
					m_items.push_back(ItemFactory::create(in.peek(), in));
			}

			void save(std::ostream& out) const override
			{
				for (const auto& v : m_items)
					v->save(out);
			}

			/// Return all item of a certain type
			template <class T>
			std::vector<std::shared_ptr<T>> get() const
			{
				std::vector<std::shared_ptr<T>> res;
				for (const auto& ptr : m_items)
				{
					const auto sptr = std::dynamic_pointer_cast<T>(ptr);
					if (sptr)
						res.push_back(std::move(sptr));
				}
				return res;
			}

			/// Remove all item of a certain type
			template <class T>
			void remove()
			{
				value_type list;
				std::copy_if(m_items.begin(), m_items.end(), std::back_inserter(list), [](const item_type& ptr) {
					return !std::dynamic_pointer_cast<T>(ptr);
				});
				m_items.swap(list);
				updateSize();
			}

			/// Sort the items by their type.
			/// This is not required by the Dicom standard but some implementations may need it.
			void sort()
			{
				std::sort(m_items.begin(), m_items.end(), [](const item_type& lhs, const item_type& rhs) {
					return lhs->type() < rhs->type();
				});
			}

		protected:
			void updateSize()
			{
				size_field_type len = m_sizeDelta;
				for (const auto& v : m_items)
					len += static_cast<size_field_type>(Object::size(*v));
				m_sizeField.set(len);
			}

			value_type m_items;
			Field<size_field_type>& m_sizeField;
			int8_t m_sizeDelta = 0;
		};
	}
}
