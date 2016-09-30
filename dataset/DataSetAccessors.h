#pragma once

#include "SparseDataSet.h"

#include <boost/optional.hpp>

namespace emds
{

	//! Test whether an existing element has integer type.
	bool isInt(const SparseDataSet& dataSet, odil::Tag tag);

	//! Return the integers contained in an existing element (read-only).
	const Value::Integers& asInt(const SparseDataSet& dataSet, odil::Tag tag);

	//! Return the integers contained in an existing element (read-write).
	Value::Integers& asInt(SparseDataSet& dataSet, odil::Tag tag);

	//! Return an integer contained in an existing element (read-only).
	const Value::Integer& asInt(const SparseDataSet& dataSet, odil::Tag tag, size_t position);

	//! Return an integer contained in an existing element (read-write).
	Value::Integer& asInt(SparseDataSet& dataSet, odil::Tag tag, size_t position);

	//! Return the first integer if it exists (element is present and not empty)
	boost::optional<const Value::Integer&> firstInt(const SparseDataSet& dataSet, odil::Tag tag);


	//! Test whether an existing element has real type.
	bool isReal(const SparseDataSet& dataSet, odil::Tag tag);

	//! Return the reals contained in an existing element (read-only).
	const Value::Reals& asReal(const SparseDataSet& dataSet, odil::Tag tag);

	//! Return the reals contained in an existing element (read-write).
	Value::Reals& asReal(SparseDataSet& dataSet, odil::Tag tag);

	//! Return an real contained in an existing element (read-only).
	const Value::Real& asReal(const SparseDataSet& dataSet, odil::Tag tag, size_t position);

	//! Return an real contained in an existing element (read-write).
	Value::Real& asReal(SparseDataSet& dataSet, odil::Tag tag, size_t position);

	//! Return the first real if it exists (element is present and not empty)
	boost::optional<const Value::Real&> firstReal(const SparseDataSet& dataSet, odil::Tag tag);


	//! Test whether an existing element has string type.
	bool isString(const SparseDataSet& dataSet, odil::Tag tag);

	//! Return the strings contained in an existing element (read-only).
	const Value::Strings& asString(const SparseDataSet& dataSet, odil::Tag tag);

	//! Return the strings contained in an existing element (read-write).
	Value::Strings& asString(SparseDataSet& dataSet, odil::Tag tag);

	//! Return a string contained in an existing element (read-only).
	const Value::String& asString(const SparseDataSet& dataSet, odil::Tag tag, size_t position);

	//! Return a string contained in an existing element (read-write).
	Value::String& asString(SparseDataSet& dataSet, odil::Tag tag, size_t position);

	//! Return the first string if it exists (element is present and not empty)
	boost::optional<const Value::String&> firstString(const SparseDataSet& dataSet, odil::Tag tag);


	//! Test whether an existing element has data set type.
	bool isDataSet(const SparseDataSet& dataSet, odil::Tag tag);

	//! Return the data sets contained in an existing element (read-only).
	const Value::DataSets& asDataSet(const SparseDataSet& dataSet, odil::Tag tag);

	//! Return the data sets contained in an existing element (read-write).
	Value::DataSets& asDataSet(SparseDataSet& dataSet, odil::Tag tag);

	//! Return a data set contained in an existing element (read-only).
	const SparseDataSet& asDataSet(const SparseDataSet& dataSet, odil::Tag tag, size_t position);

	//! Return a data set contained in an existing element (read-write).
	SparseDataSet& asDataSet(SparseDataSet& dataSet, odil::Tag tag, size_t position);

	//! Return the first data set if it exists (element is present and not empty)
	boost::optional<const SparseDataSet&> firstDataSet(const SparseDataSet& dataSet, odil::Tag tag);


	//! Test whether an existing element has binary type.
	bool isBinary(const SparseDataSet& dataSet, odil::Tag tag);

	//! Return the binary items contained in an existing element (read-only).
	const Value::Binaries& asBinary(const SparseDataSet& dataSet, odil::Tag tag);

	//! Return the binary items contained in an existing element (read-write).
	Value::Binaries& asBinary(SparseDataSet& dataSet, odil::Tag tag);

	//! Return a binary item contained in an existing element (read-only).
	const Value::Binaries::value_type& asBinary(const SparseDataSet& dataSet, odil::Tag tag, size_t position);

	//! Return a binary item contained in an existing element (read-write).
	Value::Binaries::value_type& asBinary(SparseDataSet& dataSet, odil::Tag tag, size_t position);

	//! Return the first binary item if it exists (element is present and not empty)
	boost::optional<const Value::Binaries::value_type&> firstBinary(const SparseDataSet& dataSet, odil::Tag tag);

} // namespace emds
