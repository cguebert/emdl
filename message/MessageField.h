#pragma once

#include <emdl/dataset/DataSet.h>
#include <emdl/Exception.h>
#include <emdl/SOPClasses.h>

#include <boost/variant/get.hpp>

namespace emdl
{
	namespace message
	{
		class Message;

		// Base class for all message fields
		class EMDL_API BaseField
		{
		public:
			class BaseInitField
			{
			public:
				BaseInitField(Tag tag, Message& owner, DataSet& dataSet)
					: tag(tag)
					, owner(owner)
					, dataSet(dataSet)
				{
				}
				Tag tag;
				Message& owner;
				DataSet& dataSet;

			protected:
				friend BaseField;
				void registerField(BaseField& field) const;
			};

			template <class ValueType>
			class InitField : public BaseField::BaseInitField
			{
			public:
				InitField(Tag tag, const ValueType& value, Message& owner, DataSet& dataSet)
					: BaseInitField(tag, owner, dataSet)
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

			Tag tag() const;

			virtual void copyFrom(const DataSet& dataSet) = 0;

		protected:
			Tag m_tag;
			DataSet& m_dataSet;
		};

		namespace details
		{
			template <class T>
			const T& read(const DataSet& dataSet, Tag tag)
			{
				const auto elt = dataSet[tag];
				if (!elt)
					throw Exception("No such tag {}", asString(tag));
				return boost::get<T>(elt->value().value());
			}

			template <class T>
			T& write(DataSet& dataSet, Tag tag)
			{
				if (!dataSet[tag])
					dataSet.set(tag);
				return boost::get<T>(dataSet[tag]->value().value());
			}
		}

		// Templated base class for message fields
		template <class T>
		class Field : public BaseField
		{
		public:
			using value_type = T;
			using variant_type = std::vector<T>;

			explicit Field(const BaseField::BaseInitField& init)
				: BaseField(init)
			{
			}

			const value_type& get() const
			{
				return details::read<variant_type>(m_dataSet, m_tag)[0];
			}

			void set(const value_type& val)
			{
				details::write<variant_type>(m_dataSet, m_tag) = {val};
			}
		};

		// Templated base class for vector fields
		template <class T>
		class Field<std::vector<T>> : public BaseField
		{
		public:
			using value_type = std::vector<T>;
			using variant_type = std::vector<T>;

			explicit Field(const BaseField::BaseInitField& init)
				: BaseField(init)
			{
			}

			const value_type& get() const
			{
				return details::read<variant_type>(m_dataSet, m_tag);
			}

			void set(const value_type& val)
			{
				details::write<variant_type>(m_dataSet, m_tag) = val;
			}
		};

		// Specialization for SOP_Class
		template <>
		class Field<SOP_Class> : public BaseField
		{
		public:
			using value_type = Value::String;
			using variant_type = Value::Strings;

			explicit Field(const BaseField::BaseInitField& init)
				: BaseField(init)
			{
			}

			SOP_Class get() const
			{
				return getSOPClass(details::read<variant_type>(m_dataSet, m_tag)[0]);
			}

			void set(SOP_Class val)
			{
				details::write<variant_type>(m_dataSet, m_tag) = {getSOPClassUID(val)};
			}
		};

		// Templated class for mandatory fields
		template <class T>
		class MandatoryField : public Field<T>
		{
		public:
			using value_type = typename Field<T>::value_type;
			using variant_type = typename Field<T>::variant_type;

			explicit MandatoryField(const BaseField::BaseInitField& init)
				: Field<T>(init)
			{
			}

			template <class U>
			explicit MandatoryField(const BaseField::InitField<U>& init)
				: Field<T>(init)
			{
				set(init.value);
			}

			void copyFrom(const DataSet& dataSet) override
			{
				details::write<variant_type>(BaseField::m_dataSet, BaseField::m_tag) = details::read<variant_type>(dataSet, BaseField::m_tag);
			}
		};

		// Templated class for optional fields
		template <class T>
		class OptionalField : public Field<T>
		{
		public:
			using value_type = typename Field<T>::value_type;
			using variant_type = typename Field<T>::variant_type;

			explicit OptionalField(const BaseField::BaseInitField& init)
				: Field<T>(init)
			{
			}

			template <class U>
			explicit OptionalField(const BaseField::InitField<U>& init)
				: Field<T>(init)
			{
				set(init.value);
			}

			void copyFrom(const DataSet& dataSet) override
			{
				auto element = dataSet[BaseField::m_tag];
				if (element && !element->empty()) // Copy only if non null
					details::write<variant_type>(BaseField::m_dataSet, BaseField::m_tag) = details::read<variant_type>(dataSet, BaseField::m_tag);
			}

			bool isPresent() const
			{
				return BaseField::m_dataSet.has(BaseField::m_tag);
			}

			void remove()
			{
				return BaseField::m_dataSet.remove(BaseField::m_tag);
			}
		};
	}
}
