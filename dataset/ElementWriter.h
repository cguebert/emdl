#pragma once

#include "BaseWriter.h"
#include "Element.h"

#include <ostream>

namespace emds
{

//! Write DICOM objects to a stream
class ElementWriter : public BaseWriter
{
public:
	ElementWriter(std::ostream& stream, bool explicitTS, ItemEncoding itemEncoding = ItemEncoding::ExplicitLength);

	//! Write an element (VR, VL and value)
	void writeElement(const Element& element);	

private:
	class WriterVisitor : public BaseWriter
	{
	public:
		using result_type = void;

		WriterVisitor(std::ostream& stream, odil::VR vr, bool explicitTS, ItemEncoding itemEncoding);

		void operator()(const boost::blank& value) const {}
		void operator()(const emds::Value::Integers& value) const;
		void operator()(const emds::Value::Reals&    value) const;
		void operator()(const emds::Value::Strings&  value) const;
		void operator()(const emds::Value::DataSets& value) const;
		void operator()(const emds::Value::Binaries& value) const;

	private:
		void writeEncapsulatedPixelData(const emds::Value::Binaries& value) const;

		const odil::VR m_vr;
	};
};

}
