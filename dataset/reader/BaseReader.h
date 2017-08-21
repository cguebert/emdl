#pragma once

#include <emdl/VR.h>
#include <emdl/ArrayView.h>
#include <emdl/Exception.h>
#include <emdl/Tag.h>
#include <emdl/TransferSyntaxes.h>

#include <memory>

namespace emdl
{
	using BinaryBuffer = std::vector<uint8_t>;
	using BinaryBufferSPtr = std::shared_ptr<BinaryBuffer>;
	using BinaryView = ArrayView<const uint8_t>;

	class EMDL_API BaseReader
	{
	public:
		BaseReader(const BinaryBufferSPtr& buffer, BinaryView view, TransferSyntax transferSyntax);
		BaseReader(const BinaryBufferSPtr& buffer, TransferSyntax transferSyntax); // Will set the view as the whole buffer area

		const BinaryBufferSPtr& buffer() const;
		const BinaryView& view() const;
		TransferSyntax transferSyntax() const;
		bool isExplicitTS() const;

		bool eof() const; //!< Cannot read anymore
		size_t offset() const; //!< Current reading offset
		void ignore(size_t offset); //!< Jump over this number of bytes

		Tag readTag(); //!< Read one tag
		std::string readString(size_t size); //!< Read one string
		uint32_t readLength(VR vr); //!< Read the length of an element

		//! Read one value of a specific type
		template <class T>
		T read()
		{
			const auto size = sizeof(T);
			if (m_view.size() < m_offset + size)
			{
				throw Exception("Could not read from stream in BaseReader::read, offset: {}, view size: {}, asked: {}",
								m_offset, m_view.size(), size);
			}

			T value;
			std::memcpy(&value, m_view.data() + m_offset, size);
			m_offset += size;
			return value;
		}

		//! Read one value of a specific type
		template <class T>
		BaseReader& operator>>(T& value)
		{
			value = read<T>();
			return *this;
		}

		BinaryView getView(size_t size); //!< Create a view starting from the current position and of a specific size

	protected:
		void setOffset(size_t offset); //!< Modify the reading offset

	private:
		size_t m_offset = 0;
		BinaryBufferSPtr m_buffer;
		BinaryView m_view;
		const TransferSyntax m_transferSyntax;
		const bool m_explicit;
	};

} // namespace emdl
