#pragma once

#include <emdl/pdu/ObjectField.h>

namespace emdl
{
	namespace pdu
	{
		// Specialization for string
		class EMDL_API FixedStringField : public BaseField
		{
		public:
			using value_type = std::string;

			explicit FixedStringField(const BaseField::BaseInitField& init, uint32_t size)
				: BaseField(init)
				, m_value(size, ' ')
				, m_size(size)
			{
			}

			value_type get() const
			{
				auto const begin = m_value.find_first_not_of(' ');
				if (begin == std::string::npos)
					return "";
				else
				{
					auto const end = m_value.find_last_not_of(' ');
					return m_value.substr(begin, end - begin + 1);
				}
			}

			void set(const value_type& val)
			{
				if (val.empty() || val.size() > 16)
					throw Exception("Invalid string in FixedStringField::set");

				m_value = val + std::string(m_size - val.size(), ' ');
			}

			uint32_t size() const override
			{
				return m_size;
			}

			void read(std::istream& in) override
			{
				in.read(reinterpret_cast<char*>(&m_value[0]), m_size);
			}

			void save(std::ostream& out) const override
			{
				out.write(reinterpret_cast<const char*>(m_value.data()), m_size);
			}

		protected:
			value_type m_value;
			uint32_t m_size = 0;
		};

		// Specialization for string where the size is held by another field
		template <class T>
		class StringField : public BaseField
		{
		public:
			using value_type = std::string;
			using size_field_type = T;

			explicit StringField(const BaseField::BaseInitField& init, Field<size_field_type>& sizeField, int8_t sizeDelta)
				: BaseField(init)
				, m_sizeField(sizeField)
				, m_sizeDelta(sizeDelta)
			{
			}

			const value_type& get() const
			{
				return m_value;
			}

			void set(const value_type& val)
			{
				m_value = val;
				m_sizeField.set(static_cast<size_field_type>(val.size() + m_sizeDelta));
			}

			uint32_t size() const override
			{
				return static_cast<uint32_t>(m_value.size());
			}

			void read(std::istream& in) override
			{
				const auto size = static_cast<size_field_type>(m_sizeField.get() - m_sizeDelta);
				m_value.resize(size);
				in.read(reinterpret_cast<char*>(&m_value[0]), size);
			}

			void save(std::ostream& out) const override
			{
				out.write(reinterpret_cast<const char*>(m_value.data()), m_value.size());
			}

		protected:
			value_type m_value;
			Field<size_field_type>& m_sizeField;
			int8_t m_sizeDelta = 0;
		};
	}
}
