#pragma once

#include <vector>
#include <utility>

#include "Handle.hpp"
#include "Tile.hpp"

namespace Engine::Tiles
{
	class Manager
	{
	public:

		Manager()
		{
			m_tiles.push_back(Tile{});
		}

		~Manager() = default;

		Handle add(Tile tile)
		{
			m_tiles.push_back(tile);
			return Handle{m_tiles.size() - 1};
		}

		const Tile& get(Handle handle) const
		{
			return m_tiles[handle.id];
		}

		Handle getEmptyHandle() const
		{
			return Handle(0);
		}

		class Iterator
		{
		public:

			Iterator& operator++()
			{
				++m_current;
				return *this;
			}

			std::pair<Handle, const Tile&> operator*()
			{
				return std::pair<Handle, const Tile&>{Handle(m_current), m_parent->m_tiles[m_current]};
			}

			bool operator!=(const Iterator& other) const
			{
				return m_current != other.m_current;
			}

		private:

			friend Manager;

			Iterator(const Manager* parent, const size_t current)
				: m_parent(parent), m_current(current) {}

			const Manager* m_parent;
			size_t m_current;
		};

		Iterator begin() const
		{
			return Iterator(this, 0);
		}

		Iterator end() const
		{
			return Iterator(this, m_tiles.size());
		}

	private:

		friend Iterator;

		std::vector<Tile> m_tiles;
	};
}