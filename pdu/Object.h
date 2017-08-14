#pragma once

#include <emdl/pdu/ObjectField.h>

#include <vector>
#include <ostream>

namespace emdl
{
	namespace pdu
	{
		//! Base class for PDU related high-level objects (PDU, items and sub-items).
		class EMDL_API Object
		{
		public:
			using Fields = std::vector<BaseField*>;

			virtual ~Object();

			const Fields& fields() const;

			uint32_t size() const;                      /// Compute the size of all fields but the first 3 (type, reserved and length)
			static uint32_t size(const Object& object); /// Compute the size of all fields

			virtual void read(std::istream& in);
			virtual void save(std::ostream& out) const;

		protected:
			friend class BaseField::BaseInitField;

			//! Add the given field to the list
			void addField(BaseField& field);

			//! Helper function to set the owner of a Field as this object, which will automatically add the Field to the list
			BaseField::BaseInitField initField(const char* name);

			//! Templated version of initField which accepts an initial value (otherwise the value is default constructed)
			template <class T>
			typename BaseField::InitField<T> initField(const char* name, const T& value)
			{
				return BaseField::InitField<T>(name, value, *this);
			}

			//! Fields specific to this object
			Fields m_fields;
		};

		EMDL_API std::ostream& operator<<(std::ostream& stream, const Object& object);
	}
}
