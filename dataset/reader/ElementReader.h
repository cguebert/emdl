#pragma once

#include <emdl/dataset/reader/BaseReader.h>
#include <emdl/dataset/DataSet.h>
#include <emdl/dataset/VR.h>

#include <odil/Tag.h>

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
		Element readElement(const DataSet& dataSet = DataSet{});

		//! Read only the VR of an element, or try to guess it for implicit transfer syntaxes.
		VR readVR(const DataSet& dataSet = DataSet{});

	protected:
		Value::Integers readIntegers(VR vr, uint32_t length);
		Value::Reals readReals(VR vr, uint32_t length);
		Value::Strings readStrings(VR vr, uint32_t length);
		Value::DataSets readDataSets(VR vr, uint32_t length);
		Value::Binaries readBinaries(VR vr, uint32_t length);

		DataSet readItem();
		Value::Binaries readEncapsulatedPixelData();
	};
}
