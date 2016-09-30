#include "Message.h"

#include <emdl/Exception.h>
#include <emdl/dataset/DataSetAccessors.h>
#include <emdl/dataset/DataSetReader.h>
#include <emds/config/logUtils.h>

#include <odil/registry.h>

namespace reg = odil::registry;

namespace emds
{

namespace message
{

	Message::Message()
		: commandField(initField(odil::registry::CommandField))
	{
		m_commandSet.set(reg::CommandDataSetType, { static_cast<int64_t>(DataSetType::ABSENT) }, odil::VR::US);
	}

	Message::Message(const SparseDataSet& commandSet)
		: commandField(initField(odil::registry::CommandField))
		, m_commandSet(commandSet)
	{
		m_commandSet.set(reg::CommandDataSetType, { static_cast<int64_t>(DataSetType::ABSENT) }, odil::VR::US);
	}

	Message::Message(SparseDataSet&& commandSet)
		: commandField(initField(odil::registry::CommandField))
		, m_commandSet(std::move(commandSet))
	{
		m_commandSet.set(reg::CommandDataSetType, { static_cast<int64_t>(DataSetType::ABSENT) }, odil::VR::US);
	}

	Message::Message(const SparseDataSet& commandSet, const SparseDataSet& dataSet)
		: commandField(initField(odil::registry::CommandField))
		, m_commandSet(commandSet)
		, m_dataSet(dataSet)
	{
		m_commandSet.set(reg::CommandDataSetType, { static_cast<int64_t>(DataSetType::PRESENT) }, odil::VR::US);
	}

	Message::Message(SparseDataSet&& commandSet, SparseDataSet&& dataSet)
		: commandField(initField(odil::registry::CommandField))
		, m_commandSet(std::move(commandSet))
		, m_dataSet(std::move(dataSet))
	{
		m_commandSet.set(reg::CommandDataSetType, { static_cast<int64_t>(DataSetType::PRESENT) }, odil::VR::US);
	}

	Message::Message(const SparseDataSet& commandSet, const BinaryBufferSPtr& dataBuffer, TransferSyntax transferSyntax)
		: commandField(initField(odil::registry::CommandField))
		, m_commandSet(commandSet)
		, m_dataBuffer(dataBuffer)
		, m_transferSyntax(transferSyntax)
	{
		m_commandSet.set(reg::CommandDataSetType, { static_cast<int64_t>(DataSetType::PRESENT) }, odil::VR::US);
	}

	Message::Message(SparseDataSet&& commandSet, const BinaryBufferSPtr& dataBuffer, TransferSyntax transferSyntax)
		: commandField(initField(odil::registry::CommandField))
		, m_commandSet(std::move(commandSet))
		, m_dataBuffer(dataBuffer)
		, m_transferSyntax(transferSyntax)
	{
		m_commandSet.set(reg::CommandDataSetType, { static_cast<int64_t>(DataSetType::PRESENT) }, odil::VR::US);
	}

	Message::~Message() = default;

	const SparseDataSet& Message::commandSet() const
	{
		return m_commandSet;
	}

	bool Message::hasDataSet() const
	{
		return (asInt(m_commandSet, reg::CommandDataSetType, 0) == DataSetType::PRESENT);
	}

	const SparseDataSet& Message::dataSet() const
	{
		if (!hasDataSet() || (!m_dataSet && !m_dataBuffer))
			throw Exception("No data set in message");
		if (!m_dataSet && m_dataBuffer)
			m_dataSet = DataSetReader(m_dataBuffer, m_transferSyntax).readDataSet();
		return *m_dataSet;
	}

	void Message::setDataSet(const SparseDataSet& dataSet)
	{
		m_dataSet = dataSet;
		asInt(m_commandSet, reg::CommandDataSetType) = { DataSetType::PRESENT };
	}

	void Message::deleteDataSet()
	{
		asInt(m_commandSet, reg::CommandDataSetType) = { DataSetType::ABSENT };
		m_dataSet.reset();
	}

	TransferSyntax Message::transferSyntax() const
	{
		if (m_dataSet)
			return m_dataSet->transferSyntax();
		return m_transferSyntax;
	}

	BinaryBufferSPtr Message::dataBuffer() const
	{
		return m_dataBuffer;
	}

	BaseField::BaseInitField Message::initField(odil::Tag tag)
	{
		return BaseField::BaseInitField(tag, *this, m_commandSet);
	}

	void Message::addField(BaseField& field)
	{
		m_fields.push_back(&field);
	}

	void Message::copyFieldsFromMessage(const Message& message)
	{
		const auto& dataSet = message.commandSet();
		for (const auto field : m_fields)
			field->copyFrom(dataSet);
	}

	void Message::copyFromMessage(const Message& message, Value::Integer commandField, DataSetRequirement dataSetRequired)
	{
		if (message.commandField.get() != commandField)
			throw Exception("Message is not a {}", log::getCommandName(commandField));

		switch (dataSetRequired)
		{
		case DataSetRequirement::Ignore:
			break;

		case DataSetRequirement::Absent:
			if (message.hasDataSet() && !message.dataSet().empty())
				throw Exception("Data set must not be present");
			deleteDataSet();
			break;

		case DataSetRequirement::Optional:
			if (message.hasDataSet())
				setDataSet(message.dataSet());
			break;

		case DataSetRequirement::Mandatory:
			if (!message.hasDataSet() || message.dataSet().empty())
				throw Exception("A data set is required for the construction of a {}", log::getCommandName(commandField));
			setDataSet(message.dataSet());
			break;
		}

		copyFieldsFromMessage(message);
	}

}

}
