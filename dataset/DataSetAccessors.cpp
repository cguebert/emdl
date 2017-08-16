#include <emdl/dataset/DataSetAccessors.h>

namespace emdl
{
	template <typename C>
	const typename C::value_type& atPos(const C& container, size_t position)
	{
		if (container.size() <= position)
			throw std::out_of_range("No such element");
		return container[position];
	}

	template <typename C>
	typename C::value_type& atPos(C& container, size_t position)
	{
		if (container.size() <= position)
			throw std::out_of_range("No such element");
		return container[position];
	}

	bool isInt(const SparseDataSet& dataSet, odil::Tag tag)
	{
		return dataSet[tag]->isInt();
	}

	const Value::Integers& asInt(const SparseDataSet& dataSet, odil::Tag tag)
	{
		return dataSet[tag]->asInt();
	}

	Value::Integers& asInt(SparseDataSet& dataSet, odil::Tag tag)
	{
		return dataSet[tag]->asInt();
	}

	const Value::Integer& asInt(const SparseDataSet& dataSet, odil::Tag tag, size_t position)
	{
		return atPos(asInt(dataSet, tag), position);
	}

	Value::Integer& asInt(SparseDataSet& dataSet, odil::Tag tag, size_t position)
	{
		return atPos(asInt(dataSet, tag), position);
	}

	boost::optional<const Value::Integer&> firstInt(const SparseDataSet& dataSet, odil::Tag tag)
	{
		const auto elt = dataSet[tag];
		if (elt && !elt->empty())
			return elt->asInt()[0];
		return {};
	}

	const Value::Integers& intList(const SparseDataSet& dataSet, odil::Tag tag)
	{
		const auto elt = dataSet[tag];
		if (elt)
			return elt->asInt();
		static const Value::Integers emptyList;
		return emptyList;
	}

	/*****************************************************************************/

	bool isReal(const SparseDataSet& dataSet, odil::Tag tag)
	{
		return dataSet[tag]->isReal();
	}

	const Value::Reals& asReal(const SparseDataSet& dataSet, odil::Tag tag)
	{
		return dataSet[tag]->asReal();
	}

	Value::Reals& asReal(SparseDataSet& dataSet, odil::Tag tag)
	{
		return dataSet[tag]->asReal();
	}

	const Value::Real& asReal(const SparseDataSet& dataSet, odil::Tag tag, size_t position)
	{
		return atPos(asReal(dataSet, tag), position);
	}

	Value::Real& asReal(SparseDataSet& dataSet, odil::Tag tag, size_t position)
	{
		return atPos(asReal(dataSet, tag), position);
	}

	boost::optional<const Value::Real&> firstReal(const SparseDataSet& dataSet, odil::Tag tag)
	{
		const auto elt = dataSet[tag];
		if (elt && !elt->empty())
			return elt->asReal()[0];
		return {};
	}

	const Value::Reals& realList(const SparseDataSet& dataSet, odil::Tag tag)
	{
		const auto elt = dataSet[tag];
		if (elt)
			return elt->asReal();
		static const Value::Reals emptyList;
		return emptyList;
	}

	/*****************************************************************************/

	bool isString(const SparseDataSet& dataSet, odil::Tag tag)
	{
		return dataSet[tag]->isString();
	}

	const Value::Strings& asString(const SparseDataSet& dataSet, odil::Tag tag)
	{
		return dataSet[tag]->asString();
	}

	Value::Strings& asString(SparseDataSet& dataSet, odil::Tag tag)
	{
		return dataSet[tag]->asString();
	}

	const Value::String& asString(const SparseDataSet& dataSet, odil::Tag tag, size_t position)
	{
		return atPos(asString(dataSet, tag), position);
	}

	Value::String& asString(SparseDataSet& dataSet, odil::Tag tag, size_t position)
	{
		return atPos(asString(dataSet, tag), position);
	}

	boost::optional<const Value::String&> firstString(const SparseDataSet& dataSet, odil::Tag tag)
	{
		const auto elt = dataSet[tag];
		if (elt && !elt->empty())
			return elt->asString()[0];
		return {};
	}

	const Value::Strings& stringList(const SparseDataSet& dataSet, odil::Tag tag)
	{
		const auto elt = dataSet[tag];
		if (elt)
			return elt->asString();
		static const Value::Strings emptyList;
		return emptyList;
	}

	/*****************************************************************************/

	bool isDataSet(const SparseDataSet& dataSet, odil::Tag tag)
	{
		return dataSet[tag]->isDataSet();
	}

	const Value::DataSets& asDataSet(const SparseDataSet& dataSet, odil::Tag tag)
	{
		return dataSet[tag]->asDataSet();
	}

	Value::DataSets& asDataSet(SparseDataSet& dataSet, odil::Tag tag)
	{
		return dataSet[tag]->asDataSet();
	}

	const SparseDataSet& asDataSet(const SparseDataSet& dataSet, odil::Tag tag, size_t position)
	{
		return atPos(asDataSet(dataSet, tag), position);
	}

	SparseDataSet& asDataSet(SparseDataSet& dataSet, odil::Tag tag, size_t position)
	{
		return atPos(asDataSet(dataSet, tag), position);
	}

	boost::optional<const SparseDataSet&> firstDataSet(const SparseDataSet& dataSet, odil::Tag tag)
	{
		const auto elt = dataSet[tag];
		if (elt && !elt->empty())
			return elt->asDataSet()[0];
		return {};
	}

	const Value::DataSets& dataSetList(const SparseDataSet& dataSet, odil::Tag tag)
	{
		const auto elt = dataSet[tag];
		if (elt)
			return elt->asDataSet();
		static const Value::DataSets emptyList;
		return emptyList;
	}

	/*****************************************************************************/

	bool isBinary(const SparseDataSet& dataSet, odil::Tag tag)
	{
		return dataSet[tag]->isBinary();
	}

	const Value::Binaries& asBinary(const SparseDataSet& dataSet, odil::Tag tag)
	{
		return dataSet[tag]->asBinary();
	}

	Value::Binaries& asBinary(SparseDataSet& dataSet, odil::Tag tag)
	{
		return dataSet[tag]->asBinary();
	}

	const Value::Binaries::value_type& asBinary(const SparseDataSet& dataSet, odil::Tag tag, size_t position)
	{
		return atPos(asBinary(dataSet, tag), position);
	}

	Value::Binaries::value_type& asBinary(SparseDataSet& dataSet, odil::Tag tag, size_t position)
	{
		return atPos(asBinary(dataSet, tag), position);
	}

	boost::optional<const Value::Binaries::value_type&> firstBinary(const SparseDataSet& dataSet, odil::Tag tag)
	{
		const auto elt = dataSet[tag];
		if (elt && !elt->empty())
			return elt->asBinary()[0];
		return {};
	}

	const Value::Binaries& binaryList(const SparseDataSet& dataSet, odil::Tag tag)
	{
		const auto elt = dataSet[tag];
		if (elt)
			return elt->asBinary();
		static const Value::Binaries emptyList;
		return emptyList;
	}

} // namespace emdl
