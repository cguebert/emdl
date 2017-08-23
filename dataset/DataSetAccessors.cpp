#include <emdl/dataset/DataSetAccessors.h>
#include <emdl/Exception.h>

namespace emdl
{
	template <typename C>
	const typename C::value_type& atPos(const C& container, size_t position)
	{
		if (container.size() <= position)
			throw Exception("No such element");
		return container[position];
	}

	template <typename C>
	typename C::value_type& atPos(C& container, size_t position)
	{
		if (container.size() <= position)
			throw Exception("No such element");
		return container[position];
	}

	bool isInt(const DataSet& dataSet, Tag tag)
	{
		const auto elt = dataSet[tag];
		if (!elt)
			throw Exception("No such tag {}", asString(tag));
		return elt->isInt();
	}

	const Value::Integers& asInt(const DataSet& dataSet, Tag tag)
	{
		const auto elt = dataSet[tag];
		if (!elt)
			throw Exception("No such tag {}", asString(tag));
		return elt->asInt();
	}

	Value::Integers& asInt(DataSet& dataSet, Tag tag)
	{
		const auto elt = dataSet[tag];
		if (!elt)
			throw Exception("No such tag {}", asString(tag));
		return elt->asInt();
	}

	const Value::Integer& asInt(const DataSet& dataSet, Tag tag, size_t position)
	{
		return atPos(asInt(dataSet, tag), position);
	}

	Value::Integer& asInt(DataSet& dataSet, Tag tag, size_t position)
	{
		return atPos(asInt(dataSet, tag), position);
	}

	boost::optional<const Value::Integer&> firstInt(const DataSet& dataSet, Tag tag)
	{
		const auto elt = dataSet[tag];
		if (elt && !elt->empty())
			return elt->asInt()[0];
		return {};
	}

	const Value::Integers& intList(const DataSet& dataSet, Tag tag)
	{
		const auto elt = dataSet[tag];
		if (elt)
			return elt->asInt();
		static const Value::Integers emptyList;
		return emptyList;
	}

	/*****************************************************************************/

	bool isReal(const DataSet& dataSet, Tag tag)
	{
		const auto elt = dataSet[tag];
		if (!elt)
			throw Exception("No such tag {}", asString(tag));
		return elt->isReal();
	}

	const Value::Reals& asReal(const DataSet& dataSet, Tag tag)
	{
		const auto elt = dataSet[tag];
		if (!elt)
			throw Exception("No such tag {}", asString(tag));
		return elt->asReal();
	}

	Value::Reals& asReal(DataSet& dataSet, Tag tag)
	{
		const auto elt = dataSet[tag];
		if (!elt)
			throw Exception("No such tag {}", asString(tag));
		return elt->asReal();
	}

	const Value::Real& asReal(const DataSet& dataSet, Tag tag, size_t position)
	{
		return atPos(asReal(dataSet, tag), position);
	}

	Value::Real& asReal(DataSet& dataSet, Tag tag, size_t position)
	{
		return atPos(asReal(dataSet, tag), position);
	}

	boost::optional<const Value::Real&> firstReal(const DataSet& dataSet, Tag tag)
	{
		const auto elt = dataSet[tag];
		if (elt && !elt->empty())
			return elt->asReal()[0];
		return {};
	}

	const Value::Reals& realList(const DataSet& dataSet, Tag tag)
	{
		const auto elt = dataSet[tag];
		if (elt)
			return elt->asReal();
		static const Value::Reals emptyList;
		return emptyList;
	}

	/*****************************************************************************/

	bool isString(const DataSet& dataSet, Tag tag)
	{
		const auto elt = dataSet[tag];
		if (!elt)
			throw Exception("No such tag {}", asString(tag));
		return elt->isString();
	}

	const Value::Strings& asString(const DataSet& dataSet, Tag tag)
	{
		const auto elt = dataSet[tag];
		if (!elt)
			throw Exception("No such tag {}", asString(tag));
		return elt->asString();
	}

	Value::Strings& asString(DataSet& dataSet, Tag tag)
	{
		const auto elt = dataSet[tag];
		if (!elt)
			throw Exception("No such tag {}", asString(tag));
		return elt->asString();
	}

	const Value::String& asString(const DataSet& dataSet, Tag tag, size_t position)
	{
		return atPos(asString(dataSet, tag), position);
	}

	Value::String& asString(DataSet& dataSet, Tag tag, size_t position)
	{
		return atPos(asString(dataSet, tag), position);
	}

	boost::optional<const Value::String&> firstString(const DataSet& dataSet, Tag tag)
	{
		const auto elt = dataSet[tag];
		if (elt && !elt->empty())
			return elt->asString()[0];
		return {};
	}

	const Value::Strings& stringList(const DataSet& dataSet, Tag tag)
	{
		const auto elt = dataSet[tag];
		if (elt)
			return elt->asString();
		static const Value::Strings emptyList;
		return emptyList;
	}

	/*****************************************************************************/

	bool isDataSet(const DataSet& dataSet, Tag tag)
	{
		const auto elt = dataSet[tag];
		if (!elt)
			throw Exception("No such tag {}", asString(tag));
		return elt->isDataSet();
	}

	const Value::DataSets& asDataSet(const DataSet& dataSet, Tag tag)
	{
		const auto elt = dataSet[tag];
		if (!elt)
			throw Exception("No such tag {}", asString(tag));
		return elt->asDataSet();
	}

	Value::DataSets& asDataSet(DataSet& dataSet, Tag tag)
	{
		const auto elt = dataSet[tag];
		if (!elt)
			throw Exception("No such tag {}", asString(tag));
		return elt->asDataSet();
	}

	const DataSet& asDataSet(const DataSet& dataSet, Tag tag, size_t position)
	{
		return atPos(asDataSet(dataSet, tag), position);
	}

	DataSet& asDataSet(DataSet& dataSet, Tag tag, size_t position)
	{
		return atPos(asDataSet(dataSet, tag), position);
	}

	boost::optional<const DataSet&> firstDataSet(const DataSet& dataSet, Tag tag)
	{
		const auto elt = dataSet[tag];
		if (elt && !elt->empty())
			return elt->asDataSet()[0];
		return {};
	}

	const Value::DataSets& dataSetList(const DataSet& dataSet, Tag tag)
	{
		const auto elt = dataSet[tag];
		if (elt)
			return elt->asDataSet();
		static const Value::DataSets emptyList;
		return emptyList;
	}

	/*****************************************************************************/

	bool isBinary(const DataSet& dataSet, Tag tag)
	{
		const auto elt = dataSet[tag];
		if (!elt)
			throw Exception("No such tag {}", asString(tag));
		return elt->isBinary();
	}

	const Value::Binaries& asBinary(const DataSet& dataSet, Tag tag)
	{
		const auto elt = dataSet[tag];
		if (!elt)
			throw Exception("No such tag {}", asString(tag));
		return elt->asBinary();
	}

	Value::Binaries& asBinary(DataSet& dataSet, Tag tag)
	{
		const auto elt = dataSet[tag];
		if (!elt)
			throw Exception("No such tag {}", asString(tag));
		return elt->asBinary();
	}

	const Value::Binaries::value_type& asBinary(const DataSet& dataSet, Tag tag, size_t position)
	{
		return atPos(asBinary(dataSet, tag), position);
	}

	Value::Binaries::value_type& asBinary(DataSet& dataSet, Tag tag, size_t position)
	{
		return atPos(asBinary(dataSet, tag), position);
	}

	boost::optional<const Value::Binaries::value_type&> firstBinary(const DataSet& dataSet, Tag tag)
	{
		const auto elt = dataSet[tag];
		if (elt && !elt->empty())
			return elt->asBinary()[0];
		return {};
	}

	const Value::Binaries& binaryList(const DataSet& dataSet, Tag tag)
	{
		const auto elt = dataSet[tag];
		if (elt)
			return elt->asBinary();
		static const Value::Binaries emptyList;
		return emptyList;
	}

} // namespace emdl
