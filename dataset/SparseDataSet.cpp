#include "SparseDataSet.h"
#include "ElementReader.h"

#include <odil/registry.h>

namespace emdl
{
	const uint32_t SparseDataSet::TagElementStruct::npos = static_cast<uint32_t>(-1);

	SparseDataSet::SparseDataSet(TransferSyntax transferSyntax)
		: m_transferSyntax(transferSyntax)
	{
	}

	SparseDataSet::SparseDataSet(const BinaryBufferSPtr& buffer, BinaryView view, TransferSyntax transferSyntax)
		: m_buffer(buffer)
		, m_view(view)
		, m_transferSyntax(transferSyntax)
	{
	}

	bool SparseDataSet::has(const odil::Tag& tag) const
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

	void SparseDataSet::set(const odil::Tag& tag, odil::VR vr)
	{
		if (vr == odil::VR::UNKNOWN)
			vr = odil::as_vr(tag);

		getPreparedElement(edit(tag)) = { true, Element { vr } };
	}

	void SparseDataSet::set(const odil::Tag& tag, BinaryView view)
	{
		auto& tes = edit(tag);
		tes.pos = view.data() - m_view.data();
		tes.size = view.size();
	}

	void SparseDataSet::set(const odil::Tag& tag, const Element& element)
	{
		getPreparedElement(edit(tag)) = { true, element };
	}

	void SparseDataSet::set(const odil::Tag& tag, Element&& element)
	{
		getPreparedElement(edit(tag)) = { true, std::move(element) };
	}

	void SparseDataSet::remove(const odil::Tag& tag)
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

	boost::optional<Element&> SparseDataSet::write(const odil::Tag& tag)
	{
		auto tes = find(tag);
		if (!tes)
			return {};
		return getElement(*tes);
	}

	boost::optional<Element&> SparseDataSet::operator[](const odil::Tag& tag)
	{
		return write(tag);
	}

	boost::optional<const Element&> SparseDataSet::read(const odil::Tag& tag) const
	{
		auto tes = find(tag);
		if (!tes)
			return {};
		return getElement(*tes);
	}

	boost::optional<const Element&> SparseDataSet::operator[](const odil::Tag& tag) const
	{
		return read(tag);
	}

	TransferSyntax SparseDataSet::transferSyntax() const
	{
		return m_transferSyntax;
	}

	void SparseDataSet::setTransferSyntax(TransferSyntax transferSyntax)
	{
		m_transferSyntax = transferSyntax;
	}

	bool SparseDataSet::modified() const
	{
		return m_modified;
	}

	BinaryView SparseDataSet::view() const
	{
		return m_view;
	}

	bool SparseDataSet::empty() const
	{
		return m_groups.empty();
	}

	void SparseDataSet::updateViewSize(size_t size)
	{
		m_view = BinaryView(m_view.data(), size);
	}

	SparseDataSet::const_iterator SparseDataSet::begin() const
	{
		if (m_groups.empty())
			return const_iterator(this, m_groups.begin(), {});
		return const_iterator(this, m_groups.begin(), m_groups.front().elements.begin());
	}

	SparseDataSet::const_iterator SparseDataSet::end() const
	{
		if (m_groups.empty())
			return const_iterator(this, m_groups.end(), {});
		return const_iterator(this, m_groups.end() - 1, m_groups.back().elements.end());
	}

	const SparseDataSet::Groups& SparseDataSet::getGroups() const
	{
		return m_groups;
	}

	const SparseDataSet::TagElementStruct* SparseDataSet::find(const odil::Tag& tag) const
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

	SparseDataSet::TagElementStruct& SparseDataSet::edit(const odil::Tag& tag)
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

	Element& SparseDataSet::getElement(const TagElementStruct& tes)
	{
		m_modified = true;

		return const_cast<Element&>(const_cast<const SparseDataSet*>(this)->getElement(tes, true));
	}

	const Element& SparseDataSet::getElement(const TagElementStruct& tes) const
	{
		return getElement(tes, false);
	}

	const Element& SparseDataSet::getElement(const TagElementStruct& tes, bool modified) const
	{
		// We already have a value
		if (tes.preparedIndex != TagElementStruct::npos)
			return m_preparedElements[tes.preparedIndex].second;

		// Or we must parse it and add it to the list
		tes.preparedIndex = static_cast<int>(m_preparedElements.size());
		m_preparedElements.emplace_back(modified, ElementReader { m_buffer, getView(tes), m_transferSyntax }.readElement(*this));
		return m_preparedElements.back().second;
	}

	SparseDataSet::PreparedElement& SparseDataSet::getPreparedElement(const TagElementStruct& tes) const
	{
		// We already have a value
		if (tes.preparedIndex != TagElementStruct::npos)
			return m_preparedElements[tes.preparedIndex];

		// Or we must add it to the list
		tes.preparedIndex = static_cast<int>(m_preparedElements.size());
		m_preparedElements.emplace_back(false, Element());
		return m_preparedElements.back();
	}

	BinaryView SparseDataSet::getView(const TagElementStruct& tes) const
	{
		return { m_view.data() + tes.pos, tes.size };
	}

	bool SparseDataSet::isModified(const TagElementStruct& tes) const
	{
		if (tes.preparedIndex == SparseDataSet::TagElementStruct::npos)
			return false;

		return m_preparedElements[tes.preparedIndex].first;
	}

	boost::optional<BinaryView> SparseDataSet::getView(const odil::Tag& tag) const
	{
		auto tes = find(tag);
		if (tes)
			return getView(*tes);
		else
			return {};
	}

	/*****************************************************************************/

	SparseDataSet::iterator_value::iterator_value()
	{
	}

	SparseDataSet::iterator_value::iterator_value(const SparseDataSet* dataSet, const TagElementStruct* element)
		: m_dataSet(dataSet)
		, m_element(element)
	{
	}

	odil::Tag SparseDataSet::iterator_value::tag() const
	{
		if (!m_element)
			throw std::exception("Use of an invalid SparseDataSet::iterator_value, trying to call tag()");
		return odil::Tag(m_element->tag);
	}

	const Element& SparseDataSet::iterator_value::element() const
	{
		if (!m_dataSet || !m_element)
			throw std::exception("Use of an invalid SparseDataSet::iterator_value, trying to call element()");
		return m_dataSet->getElement(*m_element);
	}

	BinaryView SparseDataSet::iterator_value::view() const
	{
		if (!m_dataSet || !m_element)
			throw std::exception("Use of an invalid SparseDataSet::iterator_value, trying to call view()");
		return m_dataSet->getView(*m_element);
	}

	/*****************************************************************************/

	SparseDataSet::const_iterator::const_iterator()
	{
	}

	SparseDataSet::const_iterator::const_iterator(const SparseDataSet* dataSet, groups_iterator grIt, elements_iterator elIt)
		: m_dataSet(dataSet)
		, m_groupsIterator(grIt)
		, m_elementsIterator(elIt)
	{
	}

	SparseDataSet::const_iterator::reference SparseDataSet::const_iterator::operator*() const
	{
		if (!m_dataSet || m_groupsIterator == m_dataSet->m_groups.end())
			throw std::exception("Deferencing an invalid SparseDataSet::const_iterator");

		return SparseDataSet::iterator_value(m_dataSet, &(*m_elementsIterator));
	}

	SparseDataSet::const_iterator& SparseDataSet::const_iterator::operator++()
	{
		if (!m_dataSet)
			throw std::exception("Use of an invalid SparseDataSet::const_iterator, operator++");

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

	SparseDataSet::const_iterator SparseDataSet::const_iterator::operator++(int)
	{
		auto tmp = *this;
		++*this;
		return tmp;
	}

	SparseDataSet::const_iterator& SparseDataSet::const_iterator::operator--()
	{
		if (!m_dataSet)
			throw std::exception("Use of an invalid SparseDataSet::const_iterator, operator--");

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

	SparseDataSet::const_iterator SparseDataSet::const_iterator::operator--(int)
	{
		auto tmp = *this;
		--*this;
		return tmp;
	}

	bool SparseDataSet::const_iterator::operator==(const const_iterator& rhs) const
	{
		return m_dataSet == rhs.m_dataSet
			&& m_groupsIterator == rhs.m_groupsIterator
			&& m_elementsIterator == rhs.m_elementsIterator;
	}

	bool SparseDataSet::const_iterator::operator!=(const const_iterator& rhs) const
	{
		return !(*this == rhs);
	}

} // namespace emdl
