#pragma once

#include <emdl/Exception.h>

#include <istream>
#include <ostream>
#include <string>
#include <vector>

namespace emdl
{
	namespace pdu
	{
		class Object;

		// Base class for all object fields
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

			virtual void read(std::istream& in) = 0;
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

			void read(std::istream& in) override
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
	}
}
