#pragma once

#include <emdl/message/Message.h>

namespace emdl
{

namespace message
{

	//! Base class for all DIMSE response messages.
	class Response: public Message
	{
	public:
		//! General status codes, from PS3.7, C
		enum Status
		{
			Success = 0x0000,
			// Warning: 0001 or Bxxx
			// Failure: Axxx or Cxxx
			Cancel = 0xFE00,
			Pending = 0xFF00,

			// Warning Status Classes, PS3.7, C.3
			AttributeListError = 0x0107,
			AttributeValueOutOfRange = 0x0116,

			// Failure Status Classes, PS3.7, C.4
			SOPClassNotSupported = 0x0122,
			ClassInstanceConflict = 0x0119,
			DuplicateSOPInstance = 0x0111,
			DuplicateInvocation = 0x0210,
			InvalidArgumentValue = 0x0115,
			InvalidAttributeValue = 0x0106,
			InvalidObjectInstance = 0x0117,
			MissingAttribute = 0x0120,
			MissingAttributeValue = 0x0121,
			MistypedArgument = 0x0212,
			NoSuchArgument = 0x0114,
			NoSuchAttribute = 0x0105,
			NoSuchEventType = 0x0113,
			NoSuchSOPInstance = 0x0112,
			NoSuchSOPClass = 0x0118,
			ProcessingFailure = 0x0110,
			ResourceLimitation = 0x0213,
			UnrecognizedOperation = 0x0211,
			NoSuchActionType = 0x0123,
			RefusedNotAuthorized = 0x0124
		};

		//! Test whether the status class is pending.
		static bool isPending(Value::Integer status);

		//! Test whether the status class is warning.
		static bool isWarning(Value::Integer status);

		//! Test whether the status class is failure.
		static bool isFailure(Value::Integer status);

		//! Create a response with given message id and status;
		Response(Value::Integer messageIdBeingRespondedTo, Value::Integer status);

		/**
		 * @brief Create a response from the Message ID Being Responded To and the Status stored in the message command set.
		 * Raise an exception is either of those elements is missing.
		 */
		Response(const Message& message);

		//! Test whether the status class is pending.
		bool isPending() const;

		//! Test whether the status class is warning.
		bool isWarning() const;

		//! Test whether the status class is failure.
		bool isFailure() const;

		//! Set the status fields (cf. PS.37, C)
		void setStatusFields(const SparseDataSet& statusFields);

		MandatoryField<Value::Integer> messageIdBeingRespondedTo;
		MandatoryField<Value::Integer> status;

		OptionalField<Value::String> offendingElement;
		OptionalField<Value::String> errorComment;
		OptionalField<Value::Integer> errorId;
		OptionalField<Value::String> affectedSopInstanceUid;
		OptionalField<Value::String> attributeIdentiferList;

	protected:
		Response();
	};

}

}
