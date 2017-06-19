#pragma once

#include <emdl/emdl_api.h>

// SparseDataSet.h is not included, to break the circular inclusion. It then needs to be included each time this file is included.
#include <emdl/BinaryValue.h>

#include <boost/variant.hpp>

namespace
{
	// Returns true if T is a Value after removing const, volatile and references
	template <class T>
	constexpr bool is_value = typename std::is_same<emdl::Value, std::remove_cv_t<std::remove_reference_t<T>>>::value;
}

namespace emdl
{

	class SparseDataSet;

	//! The value of a DICOM element
	class EMDL_API Value
	{
	public:
		//! Possible types stored in the value.
		enum class Type
		{
			Empty,
			Integers,
			Reals,
			Strings,
			DataSets,
			Binary
		};

		// Single value typedefs
		using Integer = std::int64_t;
		using Real = double;
		using String = std::string;

		// List of values typedefs
		using Integers = std::vector<Integer>;
		using Reals = std::vector<Real>;
		using Strings = std::vector<String>;
		using DataSets = std::vector<SparseDataSet>;
		using Binaries = std::vector<BinaryValue>;

		// Type of the variant used to store the value
		using ValueVariant = boost::variant<
			boost::blank,
			Integers,
			Reals,
			Strings,
			DataSets,
			Binaries
		>;

		Value() = default; //!< Build an empty value.

		//! Copy another value
		Value(const Value& value) = default;

		//! Move another value
		Value(Value&& value) = default;

		//! Build a value from existing data
		template <class T, std::enable_if_t<!is_value<T>, bool> = true>
		Value(T&& value)
			: m_value(std::forward<T>(value)) {}

		//! Copy another value
		Value& operator=(const Value& value) = default;

		//! Move another value
		Value& operator=(Value&& value) = default;

		//! Copy a value from existing data
		template <class T>
		std::enable_if_t<!is_value<T>, Value&> operator=(T&& value)
		{ m_value = std::forward<T>(value); return *this; }

		Type type() const; //!< Return the type store in the value.
		bool empty() const; //!< Test whether the value is empty.

		// These accessors throw a boost::bad_get exception if the incorrect type is asked
		Integers& asIntegers();
		const Integers& asIntegers() const;

		Reals& asReals();
		const Reals& asReals() const;

		Strings& asStrings();
		const Strings& asStrings() const;

		DataSets& asDataSets();
		const DataSets& asDataSets() const;

		Binaries& asBinaries();
		const Binaries& asBinaries() const;

		// Access to the variant
		ValueVariant& value();
		const ValueVariant& value() const;

		// Apply a visitor of values
		template <class Visitor>
		typename Visitor::result_type applyVisitor(const Visitor& visitor) const
		{ return boost::apply_visitor(visitor, m_value); }

		template <class Visitor>
		typename Visitor::result_type applyModifyingVisitor(Visitor& visitor)
		{ return boost::apply_visitor(visitor, m_value); }

	private:
		ValueVariant m_value;
	};

}
