#pragma once

#include <vector>
#include <limits>
#include <algorithm>
#include <utility>
#include <cassert>

namespace DataStructures
{
	template <typename T, size_t CAPACITY>
	class SparseSet
	{
	public:
		SparseSet() : m_Sparse(CAPACITY, k_Empty)
		{
			m_Dense.reserve(CAPACITY);
			m_Data.reserve(CAPACITY);
		}

		void insert(size_t index, const T& item)
		{
			assert(index < CAPACITY);
			assert(m_Sparse[index] == k_Empty);

			m_Data.push_back(item);
			m_Dense.push_back(index);
			m_Sparse[index] = m_Data.size() - 1;
		}

		template <typename ... Args>
		void emplace(size_t index, Args&&... args)
		{
			assert(index < CAPACITY);
			assert(m_Sparse[index] == k_Empty);

			m_Data.emplace_back(std::forward<Args>(args)...);
			m_Dense.push_back(index);
			m_Sparse[index] = m_Data.size() - 1;
		}

		bool contains(size_t index) const
		{
			return m_Sparse[index] != k_Empty;
		}

		void pop(size_t index)
		{
			assert(index < CAPACITY);

			const size_t denseIndex = m_Sparse[index];

			if (denseIndex == m_Data.size() - 1)
			{
				m_Sparse[index] = k_Empty;
				m_Dense.pop_back();
				m_Data.pop_back();
				return;
			}

			const size_t lastDenseIndex = m_Data.size() - 1;
			const size_t lastSparseIndex = m_Dense[lastDenseIndex];

			std::swap(m_Data[denseIndex], m_Data.back());

			m_Dense[denseIndex] = lastSparseIndex;
			m_Sparse[lastSparseIndex] = denseIndex;

			m_Data.pop_back();
			m_Dense.pop_back();
			m_Sparse[index] = k_Empty;
		}

		bool popIfContains(size_t index)
		{
			if (!contains(index)) return false;

			pop(index);
			return true;
		}

		const T& at(size_t index) const
		{
			assert(index < CAPACITY);
			return m_Data[m_Sparse[index]];
		}

		T& operator[](size_t index)
		{
			return m_Data[m_Sparse[index]];
		}

		bool empty() const
		{
			return m_Data.empty();
		}

		void clear()
		{
			m_Sparse.assign(CAPACITY, k_Empty);
			m_Dense.clear();
			m_Data.clear();
		}

		class Iterator
		{
		public:

			Iterator(SparseSet* set, size_t index) : m_Index(index), m_Set(set) {}

			size_t index() const
			{
				return m_Index;
			}

			T& data() const
			{
				return m_Set->m_Data[m_Index];
			}

			std::pair<size_t, T&> operator*() const
			{
				return { m_Set->m_Dense[m_Index], m_Set->m_Data[m_Index] };
			}

			Iterator& operator++()
			{
				++m_Index;
				return *this;
			}

			bool operator==(const Iterator& o) const
			{
				return m_Index == o.m_Index && m_Set == o.m_Set;
			}

			bool operator!=(const Iterator& o) const
			{
				return !(*this == o);
			}

		private:

			friend SparseSet;
			size_t m_Index;
			SparseSet<T, CAPACITY>* m_Set;
		};

		Iterator begin()
		{
			return Iterator(this, 0);
		}

		Iterator end()
		{
			return Iterator(this, m_Data.size());
		}

		Iterator popIterator(const Iterator& it)
		{
			pop(it.m_Index);
			return it;
		}

	private:
		static constexpr size_t k_Empty = std::numeric_limits<size_t>::max();
		std::vector<size_t> m_Sparse;
		std::vector<size_t> m_Dense;
		std::vector<T> m_Data;
	};
}