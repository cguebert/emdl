#include <emdl/dataset/DataSet.h>
#include <emdl/dataset/reader/ElementReader.h>
#include <emdl/Exception.h>

namespace emdl
{
	const uint32_t DataSet::TagElementStruct::npos = static_cast<uint32_t>(-1);

	DataSet::DataSet(TransferSyntax transferSyntax)
		: m_transferSyntax(transferSyntax)
	{
	}

	DataSet::DataSet(const BinaryBufferSPtr& buffer, BinaryView view, TransferSyntax transferSyntax)
		: m_buffer(buffer)
		, m_view(view)
		, m_transferSyntax(transferSyntax)
	{
	}

	bool DataSet::has(const Tag& tag) const
	{
		auto itG = std::find_if(m_groups.begin(), m_groups.end(), [tag](const Group& g) {
			return tag.group == g.group;
		});
		if (itG == m_groups.end())
			return false;

		const auto& elements = itG->elements;
		auto itE = std::find_if(elements.begin(), elements.end(), [tag](const TagElementStruct& elt) {
			return tag.element == elt.tag.element;
		});

		return itE != elements.end();
	}

	void DataSet::set(const Tag& tag, VR vr)
	{
		if (vr == VR::Unknown)
			vr = findVR(tag);

		getPreparedElement(edit(tag)) = {true, Element{vr}};
	}

	void DataSet::set(const Tag& tag, BinaryView view)
	{
		auto& tes = edit(tag);
		tes.pos = view.data() - m_view.data();
		tes.size = view.size();
	}

	void DataSet::set(const Tag& tag, const Element& element)
	{
		getPreparedElement(edit(tag)) = {true, element};
	}

	void DataSet::set(const Tag& tag, Element&& element)
	{
		getPreparedElement(edit(tag)) = {true, std::move(element)};
	}

	void DataSet::remove(const Tag& tag)
	{
		auto itG = std::find_if(m_groups.begin(), m_groups.end(), [tag](const Group& g) {
			return tag.group == g.group;
		});
		if (itG == m_groups.end())
			return;

		auto& elements = itG->elements;
		auto itE = std::find_if(elements.begin(), elements.end(), [tag](const TagElementStruct& elt) {
			return tag.element == elt.tag.element;
		});

		if (itE == elements.end())
			return;
		elements.erase(itE);

		// We can remove the group if there are no more elements in it
		if (elements.empty())
			m_groups.erase(itG);
	}

	boost::optional<Element&> DataSet::write(const Tag& tag)
	{
		auto tes = find(tag);
		if (!tes)
			return {};
		return getElement(*tes);
	}

	boost::optional<Element&> DataSet::operator[](const Tag& tag)
	{
		return write(tag);
	}

	boost::optional<const Element&> DataSet::read(const Tag& tag) const
	{
		auto tes = find(tag);
		if (!tes)
			return {};
		return getElement(*tes);
	}

	boost::optional<const Element&> DataSet::operator[](const Tag& tag) const
	{
		return read(tag);
	}

	TransferSyntax DataSet::transferSyntax() const
	{
		return m_transferSyntax;
	}

	void DataSet::setTransferSyntax(TransferSyntax transferSyntax)
	{
		m_transferSyntax = transferSyntax;
	}

	bool DataSet::modified() const
	{
		return m_modified;
	}

	BinaryView DataSet::view() const
	{
		return m_view;
	}

	bool DataSet::empty() const
	{
		return m_groups.empty();
	}

	void DataSet::updateViewSize(size_t size)
	{
		m_view = BinaryView(m_view.data(), size);
	}

	DataSet::const_iterator DataSet::begin() const
	{
		if (m_groups.empty())
			return const_iterator(this, m_groups.begin(), {});
		return const_iterator(this, m_groups.begin(), m_groups.front().elements.begin());
	}

	DataSet::const_iterator DataSet::end() const
	{
		if (m_groups.empty())
			return const_iterator(this, m_groups.end(), {});
		return const_iterator(this, m_groups.end() - 1, m_groups.back().elements.end());
	}

	const DataSet::Groups& DataSet::getGroups() const
	{
		return m_groups;
	}

	const DataSet::TagElementStruct* DataSet::find(const Tag& tag) const
	{
		auto itG = std::find_if(m_groups.begin(), m_groups.end(), [tag](const Group& g) {
			return tag.group == g.group;
		});
		if (itG == m_groups.end())
			return nullptr;

		auto& elements = itG->elements;
		auto itE = std::find_if(elements.begin(), elements.end(), [tag](const TagElementStruct& elt) {
			return tag.element == elt.tag.element;
		});

		if (itE == elements.end())
			return nullptr;
		return &(*itE);
	}

	DataSet::TagElementStruct& DataSet::edit(const Tag& tag)
	{
		auto itG = std::lower_bound(m_groups.begin(), m_groups.end(), tag.group, [](const Group& g, uint16_t id) {
			return g.group < id;
		});
		if (itG == m_groups.end() || itG->group != tag.group)
		{
			Group group;
			group.group = tag.group;

			// Insert the group so that the list stays sorted
			itG = m_groups.insert(itG, std::move(group));
		}

		auto& elements = itG->elements;
		auto itE = std::lower_bound(elements.begin(), elements.end(), tag.element, [](const TagElementStruct& elt, uint16_t id) {
			return elt.tag.element < id;
		});

		if (itE != elements.end() && itE->tag.element == tag.element)
			return *itE;
		else
		{
			TagElementStruct tes;
			tes.tag = tag;

			m_modified = true;

			// Insert the structure and return a pointer to where it was inserted
			return *elements.insert(itE, tes);
		}
	}

	Element& DataSet::getElement(const TagElementStruct& tes)
	{
		m_modified = true;

		return const_cast<Element&>(const_cast<const DataSet*>(this)->getElement(tes, true));
	}

	const Element& DataSet::getElement(const TagElementStruct& tes) const
	{
		return getElement(tes, false);
	}

	const Element& DataSet::getElement(const TagElementStruct& tes, bool modified) const
	{
		// We already have a value
		if (tes.preparedIndex != TagElementStruct::npos)
			return m_preparedElements[tes.preparedIndex].second;

		// Or we must parse it and add it to the list
		tes.preparedIndex = static_cast<int>(m_preparedElements.size());
		m_preparedElements.emplace_back(modified, ElementReader{m_buffer, getView(tes), m_transferSyntax}.readElement(*this));
		return m_preparedElements.back().second;
	}

	DataSet::PreparedElement& DataSet::getPreparedElement(const TagElementStruct& tes) const
	{
		// We already have a value
		if (tes.preparedIndex != TagElementStruct::npos)
			return m_preparedElements[tes.preparedIndex];

		// Or we must add it to the list
		tes.preparedIndex = static_cast<int>(m_preparedElements.size());
		m_preparedElements.emplace_back(false, Element());
		return m_preparedElements.back();
	}

	BinaryView DataSet::getView(const TagElementStruct& tes) const
	{
		return {m_view.data() + tes.pos, tes.size};
	}

	bool DataSet::isModified(const TagElementStruct& tes) const
	{
		if (tes.preparedIndex == DataSet::TagElementStruct::npos)
			return false;

		return m_preparedElements[tes.preparedIndex].first;
	}

	boost::optional<BinaryView> DataSet::getView(const Tag& tag) const
	{
		auto tes = find(tag);
		if (tes)
			return getView(*tes);
		else
			return {};
	}

	/*****************************************************************************/

	DataSet::iterator_value::iterator_value()
	{
	}

	DataSet::iterator_value::iterator_value(const DataSet* dataSet, const TagElementStruct* element)
		: m_dataSet(dataSet)
		, m_element(element)
	{
	}

	Tag DataSet::iterator_value::tag() const
	{
		if (!m_element)
			throw Exception("Use of an invalid DataSet::iterator_value, trying to call tag()");
		return Tag(m_element->tag);
	}

	const Element& DataSet::iterator_value::element() const
	{
		if (!m_dataSet || !m_element)
			throw Exception("Use of an invalid DataSet::iterator_value, trying to call element()");
		return m_dataSet->getElement(*m_element);
	}

	BinaryView DataSet::iterator_value::view() const
	{
		if (!m_dataSet || !m_element)
			throw Exception("Use of an invalid DataSet::iterator_value, trying to call view()");
		return m_dataSet->getView(*m_element);
	}

	/*****************************************************************************/

	DataSet::const_iterator::const_iterator()
	{
	}

	DataSet::const_iterator::const_iterator(const DataSet* dataSet, groups_iterator grIt, elements_iterator elIt)
		: m_dataSet(dataSet)
		, m_groupsIterator(grIt)
		, m_elementsIterator(elIt)
	{
	}

	DataSet::const_iterator::reference DataSet::const_iterator::operator*() const
	{
		if (!m_dataSet || m_groupsIterator == m_dataSet->m_groups.end())
			throw Exception("Deferencing an invalid DataSet::const_iterator");

		return DataSet::iterator_value(m_dataSet, &(*m_elementsIterator));
	}

	DataSet::const_iterator& DataSet::const_iterator::operator++()
	{
		if (!m_dataSet)
			throw Exception("Use of an invalid DataSet::const_iterator, operator++");

		if (m_groupsIterator == m_dataSet->m_groups.end()) // When there are no groups
			return *this;

		if (m_elementsIterator == m_groupsIterator->elements.end()) // Don't go past the end
			return *this;

		++m_elementsIterator;
		if (m_elementsIterator == m_groupsIterator->elements.end())
		{
			auto groupIt = m_groupsIterator;
			++groupIt;
			if (groupIt != m_dataSet->m_groups.end())
			{
				m_groupsIterator = groupIt;
				m_elementsIterator = m_groupsIterator->elements.begin();
			}
		}

		return *this;
	}

	DataSet::const_iterator DataSet::const_iterator::operator++(int)
	{
		auto tmp = *this;
		++*this;
		return tmp;
	}

	DataSet::const_iterator& DataSet::const_iterator::operator--()
	{
		if (!m_dataSet)
			throw Exception("Use of an invalid DataSet::const_iterator, operator--");

		if (m_groupsIterator == m_dataSet->m_groups.end()) // When there are no groups
			return *this;

		if (m_elementsIterator == m_groupsIterator->elements.begin())
		{
			if (m_groupsIterator != m_dataSet->m_groups.begin())
			{
				--m_groupsIterator;
				m_elementsIterator = m_groupsIterator->elements.end() - 1;
			}
		}
		else
			--m_elementsIterator;

		return *this;
	}

	DataSet::const_iterator DataSet::const_iterator::operator--(int)
	{
		auto tmp = *this;
		--*this;
		return tmp;
	}

	bool DataSet::const_iterator::operator==(const const_iterator& rhs) const
	{
		return m_dataSet == rhs.m_dataSet
			   && m_groupsIterator == rhs.m_groupsIterator
			   && m_elementsIterator == rhs.m_elementsIterator;
	}

	bool DataSet::const_iterator::operator!=(const const_iterator& rhs) const
	{
		return !(*this == rhs);
	}

} // namespace emdl
