#pragma once

#include <emdl/dataset/SparseDataSet.h>
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
			BaseInitField(odil::Tag tag, Message& owner, SparseDataSet& dataSet)
				: tag(tag), owner(owner), dataSet(dataSet) {}
			odil::Tag tag;
			Message& owner;
			SparseDataSet& dataSet;

		protected:
			friend BaseField;
			void registerField(BaseField& field) const;
		};

		template <class ValueType>
		class InitField : public BaseField::BaseInitField
		{
		public:
			InitField(odil::Tag tag, const ValueType& value, Message& owner, SparseDataSet& dataSet)
				: BaseInitField(tag, owner, dataSet), value(value) {}
			ValueType value;
		};

		explicit BaseField(const BaseInitField& init);

		odil::Tag tag() const;

		virtual void copyFrom(const SparseDataSet& dataSet) = 0;

	protected:
		odil::Tag m_tag;
		Message& m_owner;
		SparseDataSet& m_dataSet;
	};

	namespace details
	{

		template <class T>
		const T& read(const SparseDataSet& dataSet, odil::Tag tag)
		{ return boost::get<T>(dataSet[tag]->value().value()); }

		template <class T>
		T& write(SparseDataSet& dataSet, odil::Tag tag)
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
		{ }

		const value_type& get() const
		{ return details::read<variant_type>(m_dataSet, m_tag)[0]; }

		void set(const value_type& val)
		{ details::write<variant_type>(m_dataSet, m_tag) = { val }; }
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
		{ }

		SOP_Class get() const
		{ return getSOPClass(details::read<variant_type>(m_dataSet, m_tag)[0]); }

		void set(SOP_Class val)
		{ details::write<variant_type>(m_dataSet, m_tag) = { getSOPClassUID(val) }; }
	};

	// Templated class for mandatory fields
	template <class T>
	class MandatoryField : public Field<T>
	{
	public:
		using value_type = typename Field::value_type;
		using variant_type = typename Field::variant_type;

		explicit MandatoryField(const BaseField::BaseInitField& init)
			: Field(init)
		{ }

		template<class U> explicit MandatoryField(const BaseField::InitField<U>& init)
			: Field(init)
		{ set(init.value); }

		void copyFrom(const SparseDataSet& dataSet) override
		{ details::write<variant_type>(m_dataSet, m_tag) = details::read<variant_type>(dataSet, m_tag); }
	};

	// Templated class for optional fields
	template <class T>
	class OptionalField : public Field<T>
	{
	public:
		using value_type = typename Field::value_type;
		using variant_type = typename Field::variant_type;

		explicit OptionalField(const BaseField::BaseInitField& init)
			: Field(init)
		{ }

		template<class U> explicit OptionalField(const BaseField::InitField<U>& init)
			: Field(init)
		{ set(init.value); }

		void copyFrom(const SparseDataSet& dataSet) override
		{
			auto element = dataSet[m_tag];
			if(element && !element->empty()) // Copy only if non null
				details::write<variant_type>(m_dataSet, m_tag) = details::read<variant_type>(dataSet, m_tag); 
		}

		bool isPresent()
		{ return m_dataSet.has(m_tag);}

		void remove()
		{ return m_dataSet.remove(m_tag); }
	};

}

}
