#pragma once

#include <emdl/emdl_api.h>
#include <emdl/ArrayView.h>

#include <cassert>
#include <memory>

namespace emdl
{
	// Wrapper around either a vector or an array view
	class EMDL_API BinaryValue
	{
	public:
		using vector_type = std::vector<unsigned char>;
		using view_type = ArrayView<const void>;

		BinaryValue() {}
		BinaryValue(view_type view)
			: m_view(view)
		{
		}
		BinaryValue(const vector_type& data)
			: m_data(std::make_unique<vector_type>(data))
		{
		}
		BinaryValue(vector_type&& data)
			: m_data(std::make_unique<vector_type>(std::move(data)))
		{
		}

		BinaryValue(const BinaryValue& other) // Copy constructor
		{
			m_view = other.m_view;
			if (other.m_data)
				m_data = std::make_unique<vector_type>(*other.m_data);
		}

		BinaryValue(BinaryValue&& other) // Move constructor
			: m_data(std::move(other.m_data))
			, m_view(other.m_view)
		{
		}

		BinaryValue& operator=(const BinaryValue& other) // Copy assignment
		{
			m_view = other.m_view;
			if (other.m_data)
				m_data = std::make_unique<vector_type>(*other.m_data);
			return *this;
		}

		BinaryValue& operator=(BinaryValue&& other) // Move assignment
		{
			m_view = other.m_view;
			m_data = std::move(other.m_data);
			return *this;
		}

		void set(view_type view)
		{
			m_data.reset();
			m_view = view;
		}
		void set(const vector_type& data) { m_data = std::make_unique<vector_type>(data); }
		void set(vector_type&& data) { m_data = std::make_unique<vector_type>(std::move(data)); }

		const void* data() const
		{
			if (m_data && !m_data->empty())
				return m_data->data();
			else if (!m_view.empty())
				return m_view.data();
			return nullptr;
		}

		std::size_t size() const
		{
			if (m_data && !m_data->empty())
				return m_data->size();
			else if (!m_view.empty())
				return m_view.size();
			return 0;
		}

		vector_type get() const
		{
			const auto d = data();
			const auto s = size();
			if (!d || !s)
				return {};
			const auto ptr = static_cast<const unsigned char*>(d);
			return {ptr, ptr + s};
		}

		bool empty() const { return (!m_data || m_data->empty()) && m_view.empty(); }
		bool hasOwnership() const { return m_data != nullptr; }

		void takeOwnership()
		{
			if (hasOwnership())
				return;

			assert(!m_view.empty());
			const unsigned char* data = static_cast<const unsigned char*>(m_view.data());
			m_data = std::make_unique<vector_type>();
			m_data->assign(data, data + m_view.size());
		}

	private:
		std::unique_ptr<vector_type> m_data; // We own this data
		view_type m_view; // We do not own the memory pointed by the view
	};

} // namespace emdl
