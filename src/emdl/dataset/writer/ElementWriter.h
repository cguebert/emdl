#pragma once

#include <emdl/dataset/writer/BaseWriter.h>
#include <emdl/dataset/Element.h>

#include <ostream>

namespace emdl
{
	//! Write DICOM objects to a stream
	class EMDL_API ElementWriter : public BaseWriter
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

			WriterVisitor(std::ostream& stream, VR vr, bool explicitTS, ItemEncoding itemEncoding);

			void operator()(const boost::blank&) const {}
			void operator()(const Value::Integers& value) const;
			void operator()(const Value::Reals& value) const;
			void operator()(const Value::Strings& value) const;
			void operator()(const Value::DataSets& value) const;
			void operator()(const Value::Binaries& value) const;

		private:
			void writeEncapsulatedPixelData(const Value::Binaries& value) const;

			const VR m_vr;
		};
	};
}
