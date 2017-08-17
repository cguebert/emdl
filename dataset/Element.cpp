#include <emdl/dataset/Element.h>
#include <emdl/dataset/SparseDataSet.h>

namespace
{
	struct EmptyVisitor
	{
		using result_type = bool;

		template <class T>
		bool operator()(const T& container) const
		{
			return container.empty();
		}

		bool operator()(const boost::blank&) const
		{
			return true;
		}
	};

	struct SizeVisitor
	{
		using result_type = std::size_t;

		template <class T>
		std::size_t operator()(const T& container) const
		{
			return container.size();
		}

		std::size_t operator()(const boost::blank&) const
		{
			return 0;
		}
	};
}

namespace emdl
{
	Element::Element(VR vr)
		: vr(vr)
	{
		// clang-format off
		switch (vrType(vr))
		{
		case VRType::Int:     m_value = Value::Integers(); break;
		case VRType::Real:    m_value = Value::Reals();    break;
		case VRType::String:  m_value = Value::Strings();  break;
		case VRType::Binary:  m_value = Value::Binaries(); break;
		case VRType::Dataset: m_value = Value::DataSets(); break;
		}
		// clang-format on
	}

	bool Element::empty() const
	{
		return applyVisitor(EmptyVisitor{});
	}

	std::size_t Element::size() const
	{
		return applyVisitor(SizeVisitor{});
	}

	Value& Element::value()
	{
		return m_value;
	}

	const Value& Element::value() const
	{
		return m_value;
	}

	bool Element::isInt() const
	{
		return m_value.type() == Value::Type::Integers;
	}

	bool Element::isReal() const
	{
		return m_value.type() == Value::Type::Reals;
	}

	bool Element::isString() const
	{
		return m_value.type() == Value::Type::Strings;
	}

	bool Element::isDataSet() const
	{
		return m_value.type() == Value::Type::DataSets;
	}

	bool Element::isBinary() const
	{
		return m_value.type() == Value::Type::Binary;
	}

	Value::Integers& Element::asInt()
	{
		return m_value.asIntegers();
	}

	const Value::Integers& Element::asInt() const
	{
		return m_value.asIntegers();
	}

	Value::Reals& Element::asReal()
	{
		return m_value.asReals();
	}

	const Value::Reals& Element::asReal() const
	{
		return m_value.asReals();
	}

	Value::Strings& Element::asString()
	{
		return m_value.asStrings();
	}

	const Value::Strings& Element::asString() const
	{
		return m_value.asStrings();
	}

	Value::DataSets& Element::asDataSet()
	{
		return m_value.asDataSets();
	}

	const Value::DataSets& Element::asDataSet() const
	{
		return m_value.asDataSets();
	}

	Value::Binaries& Element::asBinary()
	{
		return m_value.asBinaries();
	}

	const Value::Binaries& Element::asBinary() const
	{
		return m_value.asBinaries();
	}
}
