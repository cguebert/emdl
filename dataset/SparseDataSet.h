#pragma once

#include <emdl/dataset/Element.h>
#include <emdl/dataset/VRFinder.h>

#include <emdl/BinaryValue.h>
#include <emdl/TransferSyntaxes.h>

#include <deque>
#include <memory>
#include <string>
#include <vector>

#include <boost/optional.hpp>

namespace emdl
{
	using BinaryBuffer = std::vector<uint8_t>;
	using BinaryBufferSPtr = std::shared_ptr<BinaryBuffer>;
	using BinaryView = ArrayView<const uint8_t>;

	class Element;

	class EMDL_API SparseDataSet
	{
	public:
		explicit SparseDataSet(TransferSyntax transferSyntax = TransferSyntax::ExplicitVRLittleEndian);
		explicit SparseDataSet(const BinaryBufferSPtr& buffer, BinaryView view, TransferSyntax transferSyntax = TransferSyntax::ExplicitVRLittleEndian);

		//! Test whether an element is in the data set.
		bool has(const odil::Tag& tag) const;

		//! Add an element to the data set, or reset it if already in the data set. The type of the value created depends on the VR.
		void set(const odil::Tag& tag, VR vr = VR::Unknown);

		//! Add an element to the data set using a pointer in the raw buffer, or assign it if already in the data set
		void set(const odil::Tag& tag, BinaryView view);

		//! Add an element to the data set, or assign it if already in the data set
		void set(const odil::Tag& tag, const Element& element);

		//! Add an element to the data set, or assign it if already in the data set
		void set(const odil::Tag& tag, Element&& element);

		//! Add an element to the data set using a value, or assign it if already in the data set
		template <class T>
		void set(const odil::Tag& tag, const std::initializer_list<T>& value, VR vr = VR::Unknown)
		{
			if (vr == VR::Unknown)
				vr = findVR(tag);
			set(tag, Element(value, vr));
		}

		//! Add an element to the data set using a value, or assign it if already in the data set
		template <class T>
		void set(const odil::Tag& tag, const std::vector<T>& value, VR vr = VR::Unknown)
		{
			if (vr == VR::Unknown)
				vr = findVR(tag);
			set(tag, Element(value, vr));
		}

		//! Add an element to the data set using a value, or assign it if already in the data set
		template <class T>
		void set(const odil::Tag& tag, std::vector<T>&& value, VR vr = VR::Unknown)
		{
			if (vr == VR::Unknown)
				vr = findVR(tag);
			set(tag, Element(std::move(value), vr));
		}

		//! Remove an element from the data set (does nothing if not present)
		void remove(const odil::Tag& tag);

		//! Return the element at the given tag, or empty if is not in the data set
		boost::optional<Element&> operator[](const odil::Tag& tag);

		//! Return the element at the given tag, or empty if is not in the data set
		boost::optional<const Element&> operator[](const odil::Tag& tag) const;

		//! Return the current transfer syntax.
		TransferSyntax transferSyntax() const;

		//! Set the current transfer syntax. (does not transcode by itself)
		void setTransferSyntax(TransferSyntax transferSyntax);

		//! Returns true if the data set is different from the original buffer
		bool modified() const;

		//! Return a view into the raw content of the data set
		BinaryView view() const;

		//! Test whether the data set is empty
		bool empty() const;

		//! Used mainly to reduce the size of the view after parsing
		void updateViewSize(size_t size);

		//! Recompute the raw content of the data set taking into account the modified elements
		//	void update_raw_buffer();

		// This stores a pointer to the raw buffer, or a index into the prepard elements
		struct TagElementStruct
		{
			odil::Tag tag; // Dicom tag
			mutable uint32_t preparedIndex = static_cast<uint32_t>(-1); // If not -1, this is the index of the value to use for this tag
			size_t pos = 0, size = 0; // Position and size of the whole element in the buffer

			static const uint32_t npos; // Value for bad indices (unsigned -1)
		};
		using TagElements = std::vector<TagElementStruct>;

		// First level of indirection for faster search
		struct Group
		{
			uint16_t group = 0; // First half of the dicom tag
			TagElements elements;
		};
		using Groups = std::vector<Group>;

		// This is the class we get when deferencing an iterator
		class EMDL_API iterator_value
		{
		public:
			iterator_value();
			iterator_value(const SparseDataSet* dataSet, const TagElementStruct* element);

			odil::Tag tag() const;
			const Element& element() const;
			BinaryView view() const;

		private:
			const SparseDataSet* m_dataSet = nullptr;
			const TagElementStruct* m_element = nullptr;
		};

		class EMDL_API const_iterator
		{
		public:
			using value_type = iterator_value;
			using reference = iterator_value;
			using pointer = iterator_value*;
			using difference_type = std::ptrdiff_t;
			using iterator_category = std::bidirectional_iterator_tag;

			using groups_iterator = typename Groups::const_iterator;
			using elements_iterator = typename TagElements::const_iterator;

			const_iterator();
			const_iterator(const SparseDataSet* dataSet, groups_iterator grIt, elements_iterator elIt);

			reference operator*() const;

			const_iterator& operator++(); // Preincrement
			const_iterator operator++(int); // Postincrement

			const_iterator& operator--(); // Predecrement
			const_iterator operator--(int); // Postdecrement

			bool operator==(const const_iterator& rhs) const;
			bool operator!=(const const_iterator& rhs) const;

		private:
			const SparseDataSet* m_dataSet = nullptr;
			groups_iterator m_groupsIterator;
			elements_iterator m_elementsIterator;
		};

		//! Return an iterator to the start of the elements.
		const_iterator begin() const;

		//! Return an iterator to the end of the elements.
		const_iterator end() const;

		// Return the list of groups in the data set (and their respective elements)
		const Groups& getGroups() const;

		// Get the element corresponding to the structure (parse it from the buffer if necessary)
		Element& getElement(const TagElementStruct& tes);
		const Element& getElement(const TagElementStruct& tes) const;

		// Get the raw buffer corresponding to the structure
		BinaryView getView(const TagElementStruct& tes) const;

		// Test whether the element corresponding to the struture has been modified
		bool isModified(const TagElementStruct& tes) const;

		// Get the raw buffer corresponding to the element at this tag
		boost::optional<BinaryView> getView(const odil::Tag& tag) const;

	private:
		friend class iterator_value;
		friend class const_iterator;

		using PreparedElement = std::pair<bool, Element>; // The bool tells if the element has been modified from the value in the buffer
		using PreparedElements = std::deque<PreparedElement>; // Using a deque, so that pointers are never invalidated

		const TagElementStruct* find(const odil::Tag& tag) const; // Return null if is not present
		TagElementStruct& edit(const odil::Tag& tag); // Create the element if is not present

		boost::optional<Element&> write(const odil::Tag& tag); // Does not create the element, just gives a write access to it if present
		boost::optional<const Element&> read(const odil::Tag& tag) const; // Gives a read only access to the element if present

		const Element& getElement(const TagElementStruct& tes, bool modified) const; // Get the element (parse it from the buffer if necessary), and set the modified flag
		PreparedElement& getPreparedElement(const TagElementStruct& tes) const; // Access to the prepared element (does not parse the view if we create the prepared element)

		//! Raw content of the data set
		BinaryBufferSPtr m_buffer;

		//! View inside the raw content
		BinaryView m_view;

		//! Current transfer syntax.
		TransferSyntax m_transferSyntax;

		//! Top level of the data set
		Groups m_groups;

		//! Parsed or modified elements
		mutable PreparedElements m_preparedElements;

		//! Has the data set been modified since it was loaded?
		mutable bool m_modified = false;
	};

} // namespace emdl
