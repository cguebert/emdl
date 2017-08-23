#pragma once

#include <emdl/emdl_api.h>

// DataSet.h is not included, to break the circular inclusion. It then needs to be included each time this file is included.
#include <emdl/BinaryValue.h>

#include <boost/variant.hpp>

namespace emdl
{
	class Value;
}

namespace
{
	template <class...>
	struct is_one_of
	{
		static constexpr bool value = false;
	};

	template <class F, class S, class... T>
	struct is_one_of<F, S, T...>
	{
		static constexpr bool value = std::is_same<F, S>::value || is_one_of<F, T...>::value;
	};

	template <class F, class... T>
	struct is_one_of<F, std::tuple<T...>>
	{
		static constexpr bool value = is_one_of<F, T...>::value;
	};

	template <class T>
	std::vector<int64_t> toInt64Vector(const std::vector<T>& vec)
	{
		std::vector<int64_t> res;
		res.reserve(vec.size());
		for (const auto v : vec)
			res.push_back(v);
		return res;
	}

	std::vector<std::string> toStringVector(const std::vector<const char*>& vec)
	{
		std::vector<std::string> res;
		res.reserve(vec.size());
		for (const auto v : vec)
			res.push_back(v);
		return res;
	}
}

namespace emdl
{
	class DataSet;

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
		using DataSets = std::vector<DataSet>;
		using Binaries = std::vector<BinaryValue>;

		// Type of the variant used to store the value
		using ValueVariant = boost::variant<
			boost::blank,
			Integers,
			Reals,
			Strings,
			DataSets,
			Binaries>;

		using Types = std::tuple<
			Integer,
			Real,
			String,
			DataSet,
			BinaryValue>;

		Value() = default; //!< Build an empty value.

		//! Copy another value
		Value(const Value& value) = default;

		//! Move another value
		Value(Value&& value) = default;

		//! Build a value from an initializer list composed of one of the supported types
		template <class T, std::enable_if_t<is_one_of<T, Types>::value, bool> = true>
		Value(std::initializer_list<T> value)
			: m_value(value)
		{
		}

		//! Build a value from a vector of one of the supported types
		template <class T, std::enable_if_t<is_one_of<T, Types>::value, bool> = true>
		Value(const std::vector<T>& value)
			: m_value(value)
		{
		}

		//! Build a value from a rvalue vector of one of the supported types
		template <class T, std::enable_if_t<is_one_of<T, Types>::value, bool> = true>
		Value(std::vector<T>&& value)
			: m_value(std::move(value))
		{
		}

		//! Build a value from a list of integers (convertible to int64_t)
		template <class T, std::enable_if_t<std::numeric_limits<T>::is_integer
												&& !std::is_same<T, int64_t>::value,
											bool> = true>
		Value(const std::vector<T>& value)
			: m_value(toInt64Vector(value))
		{
		}

		//! Build a value from an initializer list of integers (convertible to int64_t)
		template <class T, std::enable_if_t<std::numeric_limits<T>::is_integer
												&& !std::is_same<T, int64_t>::value,
											bool> = true>
		Value(std::initializer_list<T> value)
			: m_value(toInt64Vector<T>(value))
		{
		}

		//! Build a value from a list of const char*
		template <class T, std::enable_if_t<std::is_same<T, const char*>::value, bool> = true>
		Value(const std::vector<T>& value)
			: m_value(toStringVector(value))
		{
		}

		//! Build a value from an initializer list of const char*
		template <class T, std::enable_if_t<std::is_same<T, const char*>::value, bool> = true>
		Value(std::initializer_list<T> value)
			: m_value(toStringVector(value))
		{
		}

		//! Copy another value
		Value& operator=(const Value& value) = default;

		//! Move another value
		Value& operator=(Value&& value) = default;

		//! Copy an initializer list composed of one of the supported types
		template <class T, std::enable_if_t<is_one_of<T, Types>::value, Value&> = true>
		Value& operator=(std::initializer_list<T> value)
		{
			m_value = value;
			return *this;
		}

		//! Copy a vector of one of the supported types
		template <class T, std::enable_if_t<is_one_of<T, Types>::value, bool> = true>
		Value& operator=(const std::vector<T>& value)
		{
			m_value = value;
			return *this;
		}

		//! Copy a rvalue vector of one of the supported types
		template <class T, std::enable_if_t<is_one_of<T, Types>::value, bool> = true>
		Value& operator=(std::vector<T>&& value)
		{
			m_value = std::move(value);
			return *this;
		}

		//! Copy a list of integers (convertible to int64_t)
		template <class T, std::enable_if_t<std::numeric_limits<T>::is_integer
												&& !std::is_same<T, int64_t>::value,
											bool> = true>
		Value& operator=(const std::vector<T>& value)
		{
			m_value = toInt64Vector(value);
			return *this;
		}

		//! Copy an initializer list of integers (convertible to int64_t)
		template <class T, std::enable_if_t<std::numeric_limits<T>::is_integer
												&& !std::is_same<T, int64_t>::value,
											bool> = true>
		Value& operator=(std::initializer_list<T> value)
		{
			m_value = toInt64Vector<T>(value);
			return *this;
		}

		//! Copy a list of const char*
		template <class T, std::enable_if_t<std::is_same<T, const char*>::value, bool> = true>
		Value& operator=(const std::vector<T>& value)
		{
			m_value = toStringVector(value);
			return *this;
		}

		//! Copy an initializer list of const char*
		template <class T, std::enable_if_t<std::is_same<T, const char*>::value, bool> = true>
		Value& operator=(std::initializer_list<T> value)
		{
			m_value = toStringVector(value);
			return *this;
		}

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
		{
			return boost::apply_visitor(visitor, m_value);
		}

		template <class Visitor>
		typename Visitor::result_type applyModifyingVisitor(Visitor& visitor)
		{
			return boost::apply_visitor(visitor, m_value);
		}

	private:
		ValueVariant m_value;
	};
}
