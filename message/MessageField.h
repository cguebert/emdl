#pragma once

#include <emdl/dataset/SparseDataSet.h>
#include <odil/Tag.h>

#include <boost/variant/get.hpp>

namespace emds
{

namespace message
{

	class Message;

	class BaseField
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
		const T& getValue(const SparseDataSet& dataSet, odil::Tag tag)
		{ return boost::get<T>(dataSet[tag]->value().value()); }

		template <class T>
		T& getValue(SparseDataSet& dataSet, odil::Tag tag)
		{
			if (!dataSet[tag])
				dataSet.set(tag);
			return boost::get<T>(dataSet[tag]->value().value()); 
		}

	}

	template <class T>
	class MandatoryField : public BaseField
	{
	public:
		using value_type = T;
		using variant_type = std::vector<T>;

		explicit MandatoryField(const BaseField::BaseInitField& init)
			: BaseField(init)
		{ }

		template<class U> explicit MandatoryField(const BaseField::InitField<U>& init)
			: BaseField(init)
		{ set(init.value); }

		value_type get() const
		{ return details::getValue<variant_type>(m_dataSet, m_tag)[0]; }

		void set(const value_type& val)
		{ details::getValue<variant_type>(m_dataSet, m_tag) = { val }; }

		void copyFrom(const SparseDataSet& dataSet) override
		{ details::getValue<variant_type>(m_dataSet, m_tag) = details::getValue<variant_type>(dataSet, m_tag); }
	};

	template <class T>
	class OptionalField : public BaseField
	{
	public:
		using value_type = T;
		using variant_type = std::vector<T>;

		explicit OptionalField(const BaseField::BaseInitField& init)
			: BaseField(init)
		{ }

		template<class U> explicit OptionalField(const BaseField::InitField<U>& init)
			: BaseField(init)
		{ set(init.value); }

		value_type get() const
		{ return details::getValue<variant_type>(m_dataSet, m_tag)[0]; }

		void set(const value_type& val)
		{ details::getValue<variant_type>(m_dataSet, m_tag) = { val }; }

		void copyFrom(const SparseDataSet& dataSet) override
		{
			auto element = dataSet[m_tag];
			if(element && !element->empty()) // Copy only if non null
				details::getValue<variant_type>(m_dataSet, m_tag) = details::getValue<variant_type>(dataSet, m_tag); 
		}

		bool isPresent()
		{ return m_dataSet.has(m_tag);}

		void remove()
		{ return m_dataSet.remove(m_tag); }
	};

}

}
