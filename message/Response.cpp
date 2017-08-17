#include <emdl/message/Response.h>

#include <odil/registry.h>

namespace reg = odil::registry;

namespace emdl
{
	namespace message
	{
		bool Response::isPending(Value::Integer status)
		{
			return (status == Response::Pending || status == 0xFF01);
		}

		bool Response::isWarning(Value::Integer status)
		{
			return (
				status == 0x0001 || (status >> 12) == 0xB || status == Response::AttributeListError || status == Response::AttributeValueOutOfRange);
		}

		bool Response::isFailure(Value::Integer status)
		{
			return (
				(status >> 12) == 0xA || (status >> 12) == 0xC || ((status >> 8) == 0x01 && !isWarning(status)) || (status >> 8) == 0x02);
		}

		Response::Response()
			: messageIdBeingRespondedTo(initField(reg::MessageIDBeingRespondedTo))
			, status(initField(reg::Status))
			, offendingElement(initField(reg::OffendingElement))
			, errorComment(initField(reg::ErrorComment))
			, errorId(initField(reg::ErrorID))
			, affectedSopInstanceUid(initField(reg::AffectedSOPInstanceUID))
			, attributeIdentiferList(initField(reg::AttributeIdentifierList))
		{
		}

		Response::Response(Value::Integer msgIdResponse, Value::Integer statusVal)
			: Response()
		{
			messageIdBeingRespondedTo.set(msgIdResponse);
			status.set(statusVal);
		}

		Response::Response(const Message& message)
			: Response()
		{
			copyFieldsFromMessage(message);
		}

		bool Response::isPending() const
		{
			return Response::isPending(status.get());
		}

		bool Response::isWarning() const
		{
			return Response::isWarning(status.get());
		}

		bool Response::isFailure() const
		{
			return Response::isFailure(status.get());
		}

		void Response::setStatusFields(const DataSet& statusFields)
		{
			for (const auto& tagElt : statusFields)
				m_commandSet.set(tagElt.tag(), tagElt.element());
		}
	}
}
