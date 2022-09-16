#pragma once
#include <algorithm>
#include <iterator>
#include <cstddef>
#include <vector>

namespace yo
{
	template<typename Key, typename Value>
	class LookupTable
	{
	public:
		using value = std::pair<Key, Value>;
		using container = std::vector<value>;

		using iterator = typename container::iterator;
		using const_iterator = typename container::const_iterator;

	public:
		LookupTable(std::initializer_list<value> initializer)
			: m_Container(initializer)
		{
			std::sort(m_Container.begin(), m_Container.end());
		}

		LookupTable(container container)
			: m_Container(std::move(container))
		{
			std::sort(m_Container.begin(), m_Container.end());
		}

	public:
		const_iterator begin() const { return m_Container.begin(); }

		const_iterator end() const { return m_Container.end(); }

		size_t size() const { return m_Container.size(); }

	public:
		template<typename LookKey>
		const_iterator find(const LookKey& lookupKey) const
		{
			const_iterator it = std::lower_bound(begin(), end(), lookupKey,
				[](const value& value, const LookKey& key) { return value.first < key; }
			);

			return it != end() && it->first == lookupKey ? it : end();
		}

	private:
		container m_Container;
	};
}