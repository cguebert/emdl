#pragma once

#include <algorithm>

namespace emds
{

	template <class T>
	const T& bound(const T& min, const T& value, const T& max)
	{
		return std::max(min, std::min(value, max));
	}

	template <class Container, class Value>
	typename Container::const_iterator find(const Container& container, const Value& value)
	{
		return std::find(std::begin(container), std::end(container), value);
	}


	template <class Container, class Value>
	bool contains(const Container& container, const Value& value)
	{
		return std::find(std::begin(container), std::end(container), value) != std::end(container);
	}

	template <class Container, class Value>
	int count(const Container& container, const Value& value)
	{
		return std::count(std::begin(container), std::end(container), value);
	}

	template <class Container, class Value>
	int indexOf(const Container& container, const Value& value)
	{
		auto it = std::find(std::begin(container), std::end(container), value);
		if (it == std::end(container))
			return -1;
		return std::distance(std::begin(container), it);
	}

	template <class Container, class Value>
	int removeAll(Container& container, const Value& value)
	{
		if (container.empty())
			return 0;
		auto last = std::remove(container.begin(), container.end(), value);
		int dist = std::distance(last, container.end());
		container.erase(last, container.end());
		return dist;
	}

	template <class Container, class Value>
	bool removeOne(Container& container, const Value& value)
	{
		auto it = std::find(container.begin(), container.end(), value);
		if (it == container.end())
			return false;

		container.erase(it);
		return true;
	}

	template <class Container, class Function>
	int removeIf(Container& container, const Function& function)
	{
		auto last = std::remove_if(container.begin(), container.end(), function);
		int dist = std::distance(last, container.end());
		container.erase(last, container.end());
		return dist;
	}

	template <class Container>
	void removeAt(Container& container, unsigned int index)
	{
		container.erase(container.begin() + index);
	}

	template <class Container1, class Container2>
	void concatenate(Container1& to, const Container2& from)
	{
		to.insert(to.end(), from.begin(), from.end());
	}

	template <class Container, class Key>
	typename Container::mapped_type valueOrDefault(Container& map, const Key& key, const typename Container::mapped_type& defaultValue = typename Container::mapped_type())
	{
		using value_type = typename Container::mapped_type;
		auto it = map.find(key);
		if (it == map.end())
			return defaultValue;
		return it->second;
	}

	template <class String>
	void replaceAll(String& val, const String& from, const String& to)
	{
		if (from.empty())
			return;

		auto fromSize = from.size(), toSize = to.size();
		size_t pos = 0;
		while ((pos = val.find(from, pos)) != String::npos)
		{
			val.replace(pos, fromSize, to);
			pos += toSize;
		}
	}

}
