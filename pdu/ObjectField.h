#pragma once

#include <emdl/emdl_api.h>
#include <boost/variant/get.hpp>

#include <istream>
#include <ostream>
#include <string>
#include <vector>

namespace emdl
{
	namespace pdu
	{
		class Object;

		// Base class for all message fields
		class EMDL_API BaseField
		{
		public:
			class BaseInitField
			{
			public:
				BaseInitField(const char* name, Object& owner)
					: name(name)
					, owner(owner)
				{
				}
				const char* name;
				Object& owner;

			protected:
				friend BaseField;
				void registerField(BaseField& field) const;
			};

			template <class ValueType>
			class InitField : public BaseField::BaseInitField
			{
			public:
				InitField(const char* name, const ValueType& value, Object& owner)
					: BaseInitField(name, owner)
					, value(value)
				{
				}
				ValueType value;
			};

			explicit BaseField(const BaseInitField& init);
			BaseField(const BaseField&) = delete;
			BaseField& operator=(const BaseField&) = delete;
			BaseField(BaseField&&) = delete;
			BaseField& operator=(BaseField&&) = delete;

			virtual ~BaseField();
			virtual uint32_t size() const = 0;

			virtual void read(std::istream& in, uint32_t size = 0) = 0;
			virtual void save(std::ostream& out) const = 0;

			const char* name() const;

		protected:
			const char* m_name;
		};

		// Templated class for object fields
		template <class T>
		class Field : public BaseField
		{
		public:
			using value_type = T;

			explicit Field(const BaseField::BaseInitField& init)
				: BaseField(init)
			{
			}

			template <class U>
			explicit Field(const BaseField::InitField<U>& init)
				: BaseField(init)
			{
				set(init.value);
			}

			const value_type& get() const
			{
				return m_value;
			}

			void set(const value_type& val)
			{
				m_value = val;
			}

			uint32_t size() const override
			{
				return sizeof(value_type);
			}

			void read(std::istream& in, uint32_t) override
			{
				in.read(reinterpret_cast<char*>(&m_value), sizeof(m_value));
			}

			void save(std::ostream& out) const override
			{
				out.write(reinterpret_cast<char const*>(&m_value), sizeof(m_value));
			}

		protected:
			value_type m_value;
		};

		// Specialization for string
		template <>
		class Field<std::string> : public BaseField
		{
		public:
			using value_type = std::string;

			explicit Field(const BaseField::BaseInitField& init)
				: BaseField(init)
			{
			}

			const value_type& get() const
			{
				return m_value;
			}

			void set(const value_type& val)
			{
				m_value = val;
			}

			uint32_t size() const override
			{
				return static_cast<uint32_t>(m_value.size());
			}

			void read(std::istream& in, uint32_t size) override
			{
				m_value.resize(size);
				in.read(reinterpret_cast<char*>(&m_value[0]), size);
			}

			void save(std::ostream& out) const override
			{
				out.write(reinterpret_cast<const char*>(m_value.data()), m_value.size());
			}

		protected:
			value_type m_value;
		};

		// Specialization for list of Objects
		template <class T>
		class Field<std::vector<T>> : public BaseField
		{
		public:
			using item_type = T;
			using value_type = std::vector<item_type>;

			explicit Field(const BaseField::BaseInitField& init)
				: BaseField(init)
			{
			}

			const value_type& get() const
			{
				return m_value;
			}

			void set(const value_type& val)
			{
				m_value = val;
			}

			uint32_t size() const override
			{
				uint32_t len = 0;
				for (const auto& obj : m_value)
					len += item_type::size(obj);
				return len;
			}

			void read(std::istream& in, uint32_t size) override
			{
				const auto begin = in.tellg();

				m_value.clear();
				while (in.tellg() - begin < size)
				{
					item_type v{};
					v.read(in);
					m_value.push_back(std::move(v));
				}
			}

			void save(std::ostream& out) const override
			{
				for (const auto& v : m_value)
					v.save(out);
			}

		protected:
			value_type m_value;
		};
	}
}
