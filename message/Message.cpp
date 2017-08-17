#include <emdl/message/Message.h>

#include <emdl/Exception.h>
#include <emdl/dataset/DataSetAccessors.h>
#include <emdl/dataset/reader/DataSetReader.h>

#include <boost/container/flat_map.hpp>
#include <odil/registry.h>

namespace reg = odil::registry;

namespace emdl
{
	namespace message
	{
		Message::Message()
			: commandField(initField(odil::registry::CommandField))
		{
			m_commandSet.set(reg::CommandDataSetType, {static_cast<int64_t>(DataSetType::ABSENT)}, VR::US);
		}

		Message::Message(const DataSet& commandSet)
			: commandField(initField(odil::registry::CommandField))
			, m_commandSet(commandSet)
		{
			m_commandSet.set(reg::CommandDataSetType, {static_cast<int64_t>(DataSetType::ABSENT)}, VR::US);
		}

		Message::Message(DataSet&& commandSet)
			: commandField(initField(odil::registry::CommandField))
			, m_commandSet(std::move(commandSet))
		{
			m_commandSet.set(reg::CommandDataSetType, {static_cast<int64_t>(DataSetType::ABSENT)}, VR::US);
		}

		Message::Message(const DataSet& commandSet, const DataSet& dataSet)
			: commandField(initField(odil::registry::CommandField))
			, m_commandSet(commandSet)
			, m_dataSet(dataSet)
		{
			m_commandSet.set(reg::CommandDataSetType, {static_cast<int64_t>(DataSetType::PRESENT)}, VR::US);
		}

		Message::Message(DataSet&& commandSet, DataSet&& dataSet)
			: commandField(initField(odil::registry::CommandField))
			, m_commandSet(std::move(commandSet))
			, m_dataSet(std::move(dataSet))
		{
			m_commandSet.set(reg::CommandDataSetType, {static_cast<int64_t>(DataSetType::PRESENT)}, VR::US);
		}

		Message::Message(const DataSet& commandSet, const BinaryBufferSPtr& dataBuffer, TransferSyntax transferSyntax)
			: commandField(initField(odil::registry::CommandField))
			, m_commandSet(commandSet)
			, m_dataBuffer(dataBuffer)
			, m_transferSyntax(transferSyntax)
		{
			m_commandSet.set(reg::CommandDataSetType, {static_cast<int64_t>(DataSetType::PRESENT)}, VR::US);
		}

		Message::Message(DataSet&& commandSet, const BinaryBufferSPtr& dataBuffer, TransferSyntax transferSyntax)
			: commandField(initField(odil::registry::CommandField))
			, m_commandSet(std::move(commandSet))
			, m_dataBuffer(dataBuffer)
			, m_transferSyntax(transferSyntax)
		{
			m_commandSet.set(reg::CommandDataSetType, {static_cast<int64_t>(DataSetType::PRESENT)}, VR::US);
		}

		Message::~Message() = default;

		const DataSet& Message::commandSet() const
		{
			return m_commandSet;
		}

		DataSet& Message::commandSet()
		{
			return m_commandSet;
		}

		bool Message::hasDataSet() const
		{
			return (asInt(m_commandSet, reg::CommandDataSetType, 0) == DataSetType::PRESENT);
		}

		const DataSet& Message::dataSet() const
		{
			if (!hasDataSet() || (!m_dataSet && !m_dataBuffer))
				throw Exception("No data set in message");
			if (!m_dataSet && m_dataBuffer)
				m_dataSet = DataSetReader{m_dataBuffer, m_transferSyntax}.readDataSet();
			return *m_dataSet;
		}

		DataSet& Message::dataSet()
		{
			return const_cast<DataSet&>(const_cast<const Message*>(this)->dataSet());
		}

		void Message::setDataSet(const DataSet& dataSet)
		{
			m_dataSet = dataSet;
			asInt(m_commandSet, reg::CommandDataSetType) = {DataSetType::PRESENT};
		}

		void Message::deleteDataSet()
		{
			asInt(m_commandSet, reg::CommandDataSetType) = {DataSetType::ABSENT};
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
				throw Exception("Message is not a {}", getCommandName(commandField));

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
					throw Exception("A data set is required for the construction of a {}", getCommandName(commandField));
				setDataSet(message.dataSet());
				break;
			}

			copyFieldsFromMessage(message);
		}

		std::string Message::getCommandName(int64_t command)
		{
			using Commands = boost::container::flat_map<int64_t, std::string>;
			static Commands commands;
			if (commands.empty())
			{
				using cmd = message::Message::Command;

#define ADD_CMD(c) commands.emplace(cmd::c, #c)
				ADD_CMD(C_STORE_RQ);
				ADD_CMD(C_STORE_RSP);
				ADD_CMD(C_FIND_RQ);
				ADD_CMD(C_FIND_RSP);
				ADD_CMD(C_CANCEL_RQ);
				ADD_CMD(C_GET_RQ);
				ADD_CMD(C_GET_RSP);
				ADD_CMD(C_MOVE_RQ);
				ADD_CMD(C_MOVE_RSP);
				ADD_CMD(C_ECHO_RQ);
				ADD_CMD(C_ECHO_RSP);
				ADD_CMD(N_EVENT_REPORT_RQ);
				ADD_CMD(N_EVENT_REPORT_RSP);
				ADD_CMD(N_GET_RQ);
				ADD_CMD(N_GET_RSP);
				ADD_CMD(N_SET_RQ);
				ADD_CMD(N_SET_RSP);
				ADD_CMD(N_ACTION_RQ);
				ADD_CMD(N_ACTION_RSP);
				ADD_CMD(N_CREATE_RQ);
				ADD_CMD(N_CREATE_RSP);
				ADD_CMD(N_DELETE_RQ);
				ADD_CMD(N_DELETE_RSP);
#undef ADD_CMD
			}

			auto it = commands.find(command);
			if (it == commands.end())
				return "Not recognized";
			else
				return it->second;
		}

		bool Message::shouldLogDataSet(int64_t command)
		{
			using cmd = message::Message::Command;

			switch (command)
			{
			case cmd::C_FIND_RQ:
			case cmd::C_FIND_RSP:
			case cmd::C_CANCEL_RQ:
			case cmd::C_GET_RQ:
			case cmd::C_GET_RSP:
			case cmd::C_MOVE_RQ:
			case cmd::C_MOVE_RSP:
			case cmd::N_EVENT_REPORT_RQ:
			case cmd::N_EVENT_REPORT_RSP:
			case cmd::N_GET_RQ:
			case cmd::N_GET_RSP:
			case cmd::N_SET_RQ:
			case cmd::N_SET_RSP:
			case cmd::N_ACTION_RQ:
			case cmd::N_ACTION_RSP:
			case cmd::N_CREATE_RQ:
			case cmd::N_CREATE_RSP:
			case cmd::N_DELETE_RQ:
			case cmd::N_DELETE_RSP:
				return true;

			case cmd::C_STORE_RQ: // no DataSet or DataSets are too big
			case cmd::C_STORE_RSP:
			case cmd::C_ECHO_RQ:
			case cmd::C_ECHO_RSP:
			default:
				return false;
			}
		}
	}
}
