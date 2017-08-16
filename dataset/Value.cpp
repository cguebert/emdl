#include <emdl/dataset/Value.h>
#include <emdl/dataset/SparseDataSet.h>

namespace emdl
{
	Value::Type Value::type() const
	{
		return static_cast<Type>(m_value.which());
	}

	bool Value::empty() const
	{
		return !m_value.which();
	}

	Value::Integers& Value::asIntegers()
	{
		return boost::get<Integers>(m_value);
	}

	const Value::Integers& Value::asIntegers() const
	{
		return boost::get<Integers>(m_value);
	}

	Value::Reals& Value::asReals()
	{
		return boost::get<Reals>(m_value);
	}

	const Value::Reals& Value::asReals() const
	{
		return boost::get<Reals>(m_value);
	}

	Value::Strings& Value::asStrings()
	{
		return boost::get<Strings>(m_value);
	}

	const Value::Strings& Value::asStrings() const
	{
		return boost::get<Strings>(m_value);
	}

	Value::DataSets& Value::asDataSets()
	{
		return boost::get<DataSets>(m_value);
	}

	const Value::DataSets& Value::asDataSets() const
	{
		return boost::get<DataSets>(m_value);
	}

	Value::Binaries& Value::asBinaries()
	{
		return boost::get<Binaries>(m_value);
	}

	const Value::Binaries& Value::asBinaries() const
	{
		return boost::get<Binaries>(m_value);
	}

	Value::ValueVariant& Value::value()
	{
		return m_value;
	}

	const Value::ValueVariant& Value::value() const
	{
		return m_value;
	}
}
