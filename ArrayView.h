#pragma once

#include <type_traits>
#include <utility>
#include <vector>

namespace emds
{

// Wrapper around continuous data. Does not do any memory management.
template <class T>
class ArrayView
{
public:
	using Type = T;

	constexpr ArrayView(std::nullptr_t) noexcept {}
	constexpr ArrayView() noexcept {}
	constexpr ArrayView(T* data, std::size_t size) noexcept : m_data(data), m_size(size) {}
	constexpr ArrayView(std::vector<T>& data) noexcept : m_data(data.data()), m_size(data.size()) {}

	template<class U, std::size_t size, class V = typename std::enable_if<std::is_convertible<U*, T*>::value>::type>
	constexpr ArrayView(U(&data)[size]) noexcept: m_data{data}, m_size{size} {}

	template<class U, class V = typename std::enable_if<std::is_convertible<U*, T*>::value>::type>
	constexpr ArrayView(ArrayView<U> array) noexcept: m_data{array}, m_size{array.size()} {}

	constexpr operator T*() const { return m_data; }
	constexpr const T* data() const { return m_data; }
	constexpr std::size_t size() const { return m_size; } // Array size (number of elements)

	constexpr bool empty() const { return !m_size; }

	constexpr T* begin() const { return m_data; }
	constexpr T* cbegin() const { return m_data; }

	T* end() const { return m_data + m_size; }
	T* cend() const { return m_data + m_size; }

private:
	T* m_data = nullptr;
	std::size_t m_size = 0;
};

// Specialization of ArrayView, compatible to ArrayView of any type.
// Size is recalculated to take into account the type size and convert to size in bytes.
template <> 
class ArrayView<const void> 
{
public:
	using Type = const void;

	constexpr ArrayView(std::nullptr_t) noexcept {}
	constexpr ArrayView() noexcept {}
	constexpr ArrayView(const void* data, std::size_t size) noexcept : m_data(data), m_size(size) {}

	template<class T>
	constexpr ArrayView(const T* data, std::size_t size) noexcept : m_data(data), m_size(size * sizeof(T)) {}

	template<class T, std::size_t size> 
	constexpr ArrayView(T(&data)[size]) noexcept: m_data(data), m_size(size * sizeof(T)) {}

	template<class T> 
	constexpr ArrayView(const ArrayView<T>& array) noexcept: m_data(array), m_size(array.size() * sizeof(T)) {}

	template<class T> 
	constexpr ArrayView(std::vector<T>& data) noexcept : m_data(data.data()), m_size(data.size() * sizeof(T)) {}

	constexpr operator const void*() const { return m_data; }

	constexpr const void* data() const { return m_data; }
	constexpr std::size_t size() const { return m_size; } // In bytes

	constexpr bool empty() const { return !m_size; }

private:
	const void* m_data = nullptr;
	std::size_t m_size = 0;
};

} // namespace emds
