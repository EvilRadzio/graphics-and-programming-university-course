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

		SparseSet()
		{
			m_sparse.fill(k_empty);
			m_dense.reserve(CAPACITY);
			m_data.reserve(CAPACITY);
		}

		void insert(size_t index, const T& item)
		{
			assert(index < CAPACITY);
			assert(m_sparse[index] == k_empty);

			m_data.push_back(item);
			m_dense.push_back(index);
			m_sparse[index] = m_data.size() - 1;
		}

		template <typename ... Args>
		void emplace(size_t index, Args&&... args)
		{
			assert(index < CAPACITY);
			assert(m_sparse[index] == k_empty);

			m_data.emplace_back(std::forward<Args>(args)...);
			m_dense.push_back(index);
			m_sparse[index] = m_data.size() - 1;
		}

		void assign(size_t index, const T& item)
		{
			assert(index < CAPACITY);
			assert(m_sparse[index] != k_empty);

			m_data[index] = item;
		}

		bool contains(size_t index) const
		{
			return m_sparse[index] != k_empty;
		}

		void insertOrAssign(size_t index, const T& item)
		{
			contains(index) ? assign(item) : insert(item);
		}

		void pop(size_t index)
		{
			assert(index < CAPACITY);

			const size_t denseIndex = m_sparse[index];

			if (denseIndex == m_data.size() - 1)
			{
				m_sparse[index] = k_empty;
				m_dense.pop_back();
				m_data.pop_back();
				return;
			}

			const size_t lastDenseIndex = m_data.size() - 1;
			const size_t lastSparseIndex = m_dense[lastDenseIndex];

			std::swap(m_data[denseIndex], m_data.back());

			m_dense[denseIndex] = lastSparseIndex;
			m_sparse[lastSparseIndex] = denseIndex;

			m_data.pop_back();
			m_dense.pop_back();
			m_sparse[index] = k_empty;
		}

		bool tryPop(size_t index)
		{
			if (!contains(index)) return false;

			pop(index);

			return true;
		}

		const T& at(size_t index) const
		{
			assert(index < CAPACITY);
			return m_data[m_sparse[index]];
		}

		T& operator[](size_t index)
		{
			assert(index < CAPACITY);
			return m_data[m_sparse[index]];
		}

		bool empty() const
		{
			return m_data.empty();
		}

		void clear()
		{
			m_sparse.fill(k_empty);
			m_dense.clear();
			m_data.clear();
		}

		size_t size() const
		{
			return m_dense.size();
		}

		size_t capacity() const
		{
			return CAPACITY;
		}

		class View
		{
		public:

			View(SparseSet* set)
				: m_set(set) {
			}

			class Iterator
			{
			public:

				Iterator(SparseSet* set, size_t index) :
					m_index(index),
					m_set(set)
				{}

				Iterator& operator++()
				{
					++m_index;
					return *this;
				}

				Iterator operator++(int)
				{
					Iterator temp = *this;
					++m_index;
					return temp;
				}

				Iterator& operator--()
				{
					--m_index;
					return *this;
				}

				Iterator operator--(int)
				{
					Iterator temp = *this;
					--m_index;
					return temp;
				}

				std::pair<size_t, T&> operator*() const
				{
					return std::pair<size_t, T&>{ m_index, (*m_set)[m_index] };
				}

				bool operator==(const Iterator& o) const
				{
					return m_index == o.m_index && m_set == o.m_set;
				}

				bool operator!=(const Iterator& o) const
				{
					return !(*this == o);
				}

			private:

				size_t m_index;
				SparseSet* m_set;
			};

			Iterator begin()
			{
				return Iterator(m_set, 0);
			}

			Iterator end()
			{
				return Iterator(m_set, m_set->size());
			}

		private:

			SparseSet* m_set;

			friend SparseSet;
		};

		View view()
		{
			return View(this);
		}

	private:
		static constexpr size_t k_empty = std::numeric_limits<size_t>::max();
		std::array<size_t, CAPACITY> m_sparse;
		std::vector<size_t> m_dense;
		std::vector<T> m_data;
	};
}