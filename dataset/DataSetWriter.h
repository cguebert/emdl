#pragma once

#include <emdl/dataset/BaseWriter.h>
#include <emdl/dataset/SparseDataSet.h>

#include <ostream>

namespace odil
{
	class Tag;
}

namespace emdl
{
	class EMDL_API DataSetWriter : public BaseWriter
	{
	public:
		DataSetWriter(std::ostream& stream, TransferSyntax transferSyntax, ItemEncoding itemEncoding = ItemEncoding::ExplicitLength);
		DataSetWriter(std::ostream& stream, bool explicitTS, ItemEncoding itemEncoding = ItemEncoding::ExplicitLength);

		void writeDataSet(const SparseDataSet& dataSet);

		static void writeFile(std::ostream& out,
							  const SparseDataSet& dataSet,
							  SparseDataSet metaInformation = SparseDataSet(),
							  ItemEncoding itemEncoding = ItemEncoding::ExplicitLength);

		//! Write the data set to a stream using the transfer syntax set of the data set
		static void writeDataSet(std::ostream& out, const SparseDataSet& dataSet, ItemEncoding itemEncoding = ItemEncoding::ExplicitLength);

	private:
		void writeElement(const Element& element) const;
		void writeElementStruct(const SparseDataSet& dataSet, const SparseDataSet::TagElementStruct& tes, bool fastWrite) const;
	};

} // namespace emdl
