#pragma once

#include <emdl/dataset/writer/BaseWriter.h>
#include <emdl/dataset/DataSet.h>

#include <ostream>

namespace emdl
{
	class EMDL_API DataSetWriter : public BaseWriter
	{
	public:
		DataSetWriter(std::ostream& stream, TransferSyntax transferSyntax, ItemEncoding itemEncoding = ItemEncoding::ExplicitLength);
		DataSetWriter(std::ostream& stream, bool explicitTS, ItemEncoding itemEncoding = ItemEncoding::ExplicitLength);

		void writeDataSet(const DataSet& dataSet);

		static void writeFile(std::ostream& out,
							  const DataSet& dataSet,
							  DataSet metaInformation = DataSet(),
							  ItemEncoding itemEncoding = ItemEncoding::ExplicitLength);

		//! Write the data set to a stream using the transfer syntax set of the data set
		static void writeDataSet(std::ostream& out, const DataSet& dataSet, ItemEncoding itemEncoding = ItemEncoding::ExplicitLength);

	private:
		void writeElement(const Element& element) const;
		void writeElementStruct(const DataSet& dataSet, const DataSet::TagElementStruct& tes, bool fastWrite) const;
	};

} // namespace emdl
