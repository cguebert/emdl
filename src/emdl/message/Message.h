#pragma once

#include <emdl/message/MessageField.h>

namespace emdl
{
	namespace message
	{
		//! Base class for all DIMSE messages.
		class EMDL_API Message
		{
		public:
			struct Command
			{
				enum Type
				{
					C_STORE_RQ = 0x0001,
					C_STORE_RSP = 0x8001,

					C_FIND_RQ = 0x0020,
					C_FIND_RSP = 0x8020,

					C_CANCEL_RQ = 0x0FFF,

					C_GET_RQ = 0x0010,
					C_GET_RSP = 0x8010,

					C_MOVE_RQ = 0x0021,
					C_MOVE_RSP = 0x8021,

					C_ECHO_RQ = 0x0030,
					C_ECHO_RSP = 0x8030,

					N_EVENT_REPORT_RQ = 0x0100,
					N_EVENT_REPORT_RSP = 0x8100,

					N_GET_RQ = 0x0110,
					N_GET_RSP = 0x8110,

					N_SET_RQ = 0x0120,
					N_SET_RSP = 0x8120,

					N_ACTION_RQ = 0x0130,
					N_ACTION_RSP = 0x8130,

					N_CREATE_RQ = 0x0140,
					N_CREATE_RSP = 0x8140,

					N_DELETE_RQ = 0x0150,
					N_DELETE_RSP = 0x8150,
				};
			};

			struct Priority
			{
				enum Type
				{
					LOW = 0x0002,
					MEDIUM = 0x0000,
					HIGH = 0x0001,
				};
			};

			struct DataSetType
			{
				enum Type
				{
					PRESENT = 0x0000,
					ABSENT = 0x0101,
				};
			};

			//! Create a message with an empty command set and an empty data set.
			Message();

			//! Create a message from existing data.
			explicit Message(const DataSet& commandSet);
			explicit Message(DataSet&& commandSet);

			//! Create a message from existing data.
			explicit Message(const DataSet& commandSet, const DataSet& dataSet);
			explicit Message(DataSet&& commandSet, DataSet&& dataSet);

			//! Create a message from existing data.
			explicit Message(const DataSet& commandSet, const BinaryBufferSPtr& dataBuffer, TransferSyntax transferSyntax);
			explicit Message(DataSet&& commandSet, const BinaryBufferSPtr& dataBuffer, TransferSyntax transferSyntax);

			//! Destructor;
			virtual ~Message();

			//! Return the command set of the message.
			const DataSet& commandSet() const;

			//! Return the command set of the message.
			DataSet& commandSet();

			//! Test whether as data set is present in the message.
			bool hasDataSet() const;

			//! Return the data set of the message, raise an exception if no data set is present.
			const DataSet& dataSet() const;

			//! Return the data set of the message, raise an exception if no data set is present.
			DataSet& dataSet();

			//! Set the data set of the message.
			void setDataSet(const DataSet& data_set);

			//! Delete the data set in this message.
			void deleteDataSet();

			//! Return the transfer syntax of the data set
			TransferSyntax transferSyntax() const;

			//! Return the buffer of the data set (before being parsed)
			BinaryBufferSPtr dataBuffer() const;

			//! Get a human friendly string for a Message::Command::Type value
			static std::string getCommandName(int64_t command);

			//! Get the log policy toward the dataset (if any) for the given commandMessage::Command::Type
			static bool shouldLogDataSet(int64_t command);

		private: // We put this variable here so that it is initialized before commandField
			//! Fields specific to this message
			std::vector<BaseField*> m_fields;

		public:
			//! Accessor to the command field of the message (designation of the DIMSE operation)
			MandatoryField<Value::Integer> commandField;

		protected:
			friend class BaseField::BaseInitField;

			//! Add the given field to the list
			void addField(BaseField& field);

			//! Automatically copy the fields from the other message
			void copyFieldsFromMessage(const Message& message);

			enum class DataSetRequirement
			{
				Ignore,
				Absent,
				Optional,
				Mandatory
			};

			//! Verify the type of the message and copy the fields
			void copyFromMessage(const Message& message, Value::Integer commandField, DataSetRequirement dataSetRequired);

			//! Helper function to set the owner of a Field as this object, which will automatically add the Field to the list
			BaseField::BaseInitField initField(Tag tag);

			//! Templated version of initField which accepts an initial value (otherwise the value is default constructed)
			template <class T>
			typename BaseField::InitField<T> initField(Tag tag, const T& value)
			{
				return BaseField::InitField<T>(tag, value, *this, m_commandSet);
			}

			//! Command set of the message.
			DataSet m_commandSet;

			//! Data set of the message.
			mutable boost::optional<DataSet> m_dataSet;

			//! Content of the data set, before being parsed
			BinaryBufferSPtr m_dataBuffer;

			//! Transfer syntax of the data set if present
			TransferSyntax m_transferSyntax = TransferSyntax::ImplicitVRLittleEndian;
		};
	}
}
