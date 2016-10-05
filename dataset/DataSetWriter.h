#pragma once

#include "BaseWriter.h"
#include "SparseDataSet.h"

#include <ostream>

namespace odil
{
	class Tag;
}

namespace emdl
{

	class DataSetWriter : public BaseWriter
	{
	public:
		DataSetWriter(std::ostream& stream, TransferSyntax transferSyntax, ItemEncoding itemEncoding = ItemEncoding::ExplicitLength);
		DataSetWriter(std::ostream& stream, bool explicitTS, ItemEncoding itemEncoding = ItemEncoding::ExplicitLength);

		static void writeFile(std::ostream& out,
							  const SparseDataSet& dataSet, 
							  SparseDataSet metaInformation = SparseDataSet(),
							  ItemEncoding itemEncoding = ItemEncoding::ExplicitLength);

		void writeDataSet(const SparseDataSet& dataSet);

	private:
		void writeElement(const Element& element) const;
		void writeElementStruct(const SparseDataSet& dataSet, const SparseDataSet::TagElementStruct& tes, bool fastWrite) const;
	};

} // namespace emdl
