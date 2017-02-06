#pragma once

#include "SparseDataSet.h"

#include <boost/optional.hpp>

namespace emdl
{

	//! Test whether an existing element has integer type.
	EMDL_API bool isInt(const SparseDataSet& dataSet, odil::Tag tag);

	//! Return the integers contained in an existing element (read-only).
	EMDL_API const Value::Integers& asInt(const SparseDataSet& dataSet, odil::Tag tag);

	//! Return the integers contained in an existing element (read-write).
	EMDL_API Value::Integers& asInt(SparseDataSet& dataSet, odil::Tag tag);

	//! Return an integer contained in an existing element (read-only).
	EMDL_API const Value::Integer& asInt(const SparseDataSet& dataSet, odil::Tag tag, size_t position);

	//! Return an integer contained in an existing element (read-write).
	EMDL_API Value::Integer& asInt(SparseDataSet& dataSet, odil::Tag tag, size_t position);

	//! Return the first integer if it exists (element is present and not empty).
	EMDL_API boost::optional<const Value::Integer&> firstInt(const SparseDataSet& dataSet, odil::Tag tag);

	//! Return the integers if they exist, or an empty list.
	EMDL_API const Value::Integers& intList(const SparseDataSet& dataSet, odil::Tag tag);

/*****************************************************************************/

	//! Test whether an existing element has real type.
	EMDL_API bool isReal(const SparseDataSet& dataSet, odil::Tag tag);

	//! Return the reals contained in an existing element (read-only).
	EMDL_API const Value::Reals& asReal(const SparseDataSet& dataSet, odil::Tag tag);

	//! Return the reals contained in an existing element (read-write).
	EMDL_API Value::Reals& asReal(SparseDataSet& dataSet, odil::Tag tag);

	//! Return an real contained in an existing element (read-only).
	EMDL_API const Value::Real& asReal(const SparseDataSet& dataSet, odil::Tag tag, size_t position);

	//! Return an real contained in an existing element (read-write).
	EMDL_API Value::Real& asReal(SparseDataSet& dataSet, odil::Tag tag, size_t position);

	//! Return the first real if it exists (element is present and not empty).
	EMDL_API boost::optional<const Value::Real&> firstReal(const SparseDataSet& dataSet, odil::Tag tag);

	//! Return the reals if they exist, or an empty list.
	EMDL_API const Value::Reals& realList(const SparseDataSet& dataSet, odil::Tag tag);

/*****************************************************************************/

	//! Test whether an existing element has string type.
	EMDL_API bool isString(const SparseDataSet& dataSet, odil::Tag tag);

	//! Return the strings contained in an existing element (read-only).
	EMDL_API const Value::Strings& asString(const SparseDataSet& dataSet, odil::Tag tag);

	//! Return the strings contained in an existing element (read-write).
	EMDL_API Value::Strings& asString(SparseDataSet& dataSet, odil::Tag tag);

	//! Return a string contained in an existing element (read-only).
	EMDL_API const Value::String& asString(const SparseDataSet& dataSet, odil::Tag tag, size_t position);

	//! Return a string contained in an existing element (read-write).
	EMDL_API Value::String& asString(SparseDataSet& dataSet, odil::Tag tag, size_t position);

	//! Return the first string if it exists (element is present and not empty).
	EMDL_API boost::optional<const Value::String&> firstString(const SparseDataSet& dataSet, odil::Tag tag);

	//! Return the strings if they exist, or an empty list.
	EMDL_API const Value::Strings& stringList(const SparseDataSet& dataSet, odil::Tag tag);

/*****************************************************************************/

	//! Test whether an existing element has data set type.
	EMDL_API bool isDataSet(const SparseDataSet& dataSet, odil::Tag tag);

	//! Return the data sets contained in an existing element (read-only).
	EMDL_API const Value::DataSets& asDataSet(const SparseDataSet& dataSet, odil::Tag tag);

	//! Return the data sets contained in an existing element (read-write).
	EMDL_API Value::DataSets& asDataSet(SparseDataSet& dataSet, odil::Tag tag);

	//! Return a data set contained in an existing element (read-only).
	EMDL_API const SparseDataSet& asDataSet(const SparseDataSet& dataSet, odil::Tag tag, size_t position);

	//! Return a data set contained in an existing element (read-write).
	EMDL_API SparseDataSet& asDataSet(SparseDataSet& dataSet, odil::Tag tag, size_t position);

	//! Return the first data set if it exists (element is present and not empty).
	EMDL_API boost::optional<const SparseDataSet&> firstDataSet(const SparseDataSet& dataSet, odil::Tag tag);

	//! Return the data sets if they exist, or an empty list.
	EMDL_API const Value::DataSets& dataSetList(const SparseDataSet& dataSet, odil::Tag tag);

/*****************************************************************************/

	//! Test whether an existing element has binary type.
	EMDL_API bool isBinary(const SparseDataSet& dataSet, odil::Tag tag);

	//! Return the binary items contained in an existing element (read-only).
	EMDL_API const Value::Binaries& asBinary(const SparseDataSet& dataSet, odil::Tag tag);

	//! Return the binary items contained in an existing element (read-write).
	EMDL_API Value::Binaries& asBinary(SparseDataSet& dataSet, odil::Tag tag);

	//! Return a binary item contained in an existing element (read-only).
	EMDL_API const Value::Binaries::value_type& asBinary(const SparseDataSet& dataSet, odil::Tag tag, size_t position);

	//! Return a binary item contained in an existing element (read-write).
	EMDL_API Value::Binaries::value_type& asBinary(SparseDataSet& dataSet, odil::Tag tag, size_t position);

	//! Return the first binary item if it exists (element is present and not empty).
	EMDL_API boost::optional<const Value::Binaries::value_type&> firstBinary(const SparseDataSet& dataSet, odil::Tag tag);

	//! Return the binary items if they exist, or an empty list.
	EMDL_API const Value::Binaries& binaryList(const SparseDataSet& dataSet, odil::Tag tag);

} // namespace emdl
