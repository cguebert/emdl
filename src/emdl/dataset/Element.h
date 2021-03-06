#pragma once

#include <emdl/dataset/Value.h>
#include <emdl/VR.h>
#include <emdl/Tag.h>

namespace emdl
{
	class Element;
}

namespace
{
	// Returns true if T is a Value after removing const, volatile and references
	template <class T>
	constexpr bool is_element = std::is_same<emdl::Element, std::remove_cv_t<std::remove_reference_t<T>>>::value;

	// Returns true if T is a std::vector
	template <class T>
	struct is_vector : std::false_type
	{
	};

	template <class T>
	struct is_vector<std::vector<T>> : std::true_type
	{
	};
}

namespace emdl
{
	//! Element of a DICOM data set
	class EMDL_API Element
	{
	public:
		//! Value Rrepresentation of the element
		VR vr = VR::Invalid;

		//! Default constructor
		Element() = default;

		//! Copy an existing element
		Element(const Element&) = default;

		//! Move an existing element
		Element(Element&&) = default;

		//! Create an element. The type of the value created depends on the VR.
		Element(VR vr);

		//!  Create an element with a rvalue vector of values
		template <class T>
		Element(std::vector<T>&& value, const VR& vr = VR::Invalid)
			: vr(vr)
			, m_value(std::move(value))
		{
		}

		//! Create an element with a vector of values
		template <class T>
		Element(const std::vector<T>& value, const VR& vr = VR::Invalid)
			: vr(vr)
			, m_value(value)
		{
		}

		//! Create an element with an initializer list of values
		template <class T, std::enable_if_t<!std::is_same<T, VR>::value && !is_vector<T>::value, bool> = true>
		Element(std::initializer_list<T> value, const VR& vr = VR::Invalid)
			: vr(vr)
			, m_value(value)
		{
		}

		// Assignement operators (as we declared custom constructors)
		Element& operator=(const Element&) = default;
		Element& operator=(Element&&) = default;

		//! Test whether the element is empty
		bool empty() const;

		//! Return the number of items in the value
		std::size_t size() const;

		// Return the value
		Value& value();
		const Value& value() const;

		// Test the type of the value
		bool isInt() const;
		bool isReal() const;
		bool isString() const;
		bool isDataSet() const;
		bool isBinary() const;

		// Accessors
		Value::Integers& asInt();
		const Value::Integers& asInt() const;

		Value::Reals& asReal();
		const Value::Reals& asReal() const;

		Value::Strings& asString();
		const Value::Strings& asString() const;

		Value::DataSets& asDataSet();
		const Value::DataSets& asDataSet() const;

		Value::Binaries& asBinary();
		const Value::Binaries& asBinary() const;

		// Apply a visitor of elements
		template <class Visitor>
		typename Visitor::result_type applyVisitor(const Visitor& visitor) const
		{
			return boost::apply_visitor(visitor, m_value.value());
		}

		template <class Visitor>
		typename Visitor::result_type applyModifyingVisitor(Visitor& visitor)
		{
			return boost::apply_visitor(visitor, m_value.value());
		}

	private:
		Value m_value;
	};
}
