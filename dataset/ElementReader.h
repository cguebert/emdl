#pragma once

#include <emdl/dataset/BaseReader.h>
#include <emdl/dataset/SparseDataSet.h>

#include <odil/Tag.h>
#include <odil/VR.h>

#include <istream>

namespace emdl
{
	//! Read DICOM elements from a view in memory
	class EMDL_API ElementReader : public BaseReader
	{
	public:
		ElementReader(const BinaryBufferSPtr& buffer, BinaryView view, TransferSyntax transferSyntax);
		ElementReader(const BinaryBufferSPtr& buffer, TransferSyntax transferSyntax);

		//! Read an element (VR and value), try to guess the VR from the tag, the partially read data set, and its transfer syntax.
		Element readElement(const SparseDataSet& dataSet = SparseDataSet{});

		//! Read only the VR of an element, or try to guess it for implicit transfer syntaxes.
		odil::VR readVR(const SparseDataSet& dataSet = SparseDataSet{});

	protected:
		Value::Integers readIntegers(odil::VR vr, uint32_t length);
		Value::Reals readReals(odil::VR vr, uint32_t length);
		Value::Strings readStrings(odil::VR vr, uint32_t length);
		Value::DataSets readDataSets(odil::VR vr, uint32_t length);
		Value::Binaries readBinaries(odil::VR vr, uint32_t length);

		SparseDataSet readItem();
		Value::Binaries readEncapsulatedPixelData();
	};
}
