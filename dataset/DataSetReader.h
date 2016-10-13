#pragma once

#include "BaseReader.h"
#include "SparseDataSet.h"

#include <functional>

namespace emdl
{

	EMDL_API BinaryBufferSPtr createBufferFromFile(const std::string& fileName);
	EMDL_API BinaryBufferSPtr createBufferFromStream(std::istream& stream);
	EMDL_API BinaryBufferSPtr createBufferFromString(const std::string& str);
	EMDL_API bool isDicomFile(const BinaryBufferSPtr& buffer);
	EMDL_API bool isDicomFile(const std::string& filePath); // Does not load the whole file

	struct EMDL_API FileSparseDataSets
	{
		SparseDataSet metaInformation;
		SparseDataSet dataSet;
	};

	class EMDL_API DataSetReader : public BaseReader
	{
	public:
		using HaltConditionFunc = std::function<bool(const odil::Tag&)>;

		DataSetReader(const BinaryBufferSPtr& buffer, BinaryView view, TransferSyntax transferSyntax);
		DataSetReader(const BinaryBufferSPtr& buffer, TransferSyntax transferSyntax);

		static FileSparseDataSets readFile(const BinaryBufferSPtr& buffer, HaltConditionFunc func = {});
		static FileSparseDataSets readFile(const std::string& filePath, HaltConditionFunc func = {});

		SparseDataSet readDataSet();
		SparseDataSet readDataSet(HaltConditionFunc haltFunc);
	
	private:
		struct ElementInfo
		{
			uint32_t length;
			odil::VR vr;
		};
		ElementInfo readElement();
		void ignoreItem();
	};

} // namespace emdl
