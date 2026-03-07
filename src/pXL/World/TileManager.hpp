#pragma once

#include <vector>
#include <utility>

#include "TileHandle.hpp"
#include "Tile.hpp"

namespace px
{
	class TileManager
	{
	public:

		TileManager()
		{
			m_tiles.push_back(Tile{});
		}

		~TileManager() = default;

		TileHandle add(Tile tile)
		{
			m_tiles.push_back(tile);
			return TileHandle{m_tiles.size() - 1};
		}

		const Tile& get(TileHandle handle) const
		{
			return m_tiles[handle.id];
		}

		TileHandle getEmptyHandle() const
		{
			return TileHandle(0);
		}

		class Iterator
		{
		public:

			Iterator& operator++()
			{
				++m_current;
				return *this;
			}

			std::pair<TileHandle, const Tile&> operator*()
			{
				return std::pair<TileHandle, const Tile&>{TileHandle(m_current), m_parent->m_tiles[m_current]};
			}

			bool operator!=(const Iterator& other) const
			{
				return m_current != other.m_current;
			}

		private:

			friend TileManager;

			Iterator(const TileManager* parent, const size_t current)
				: m_parent(parent), m_current(current) {}

			const TileManager* m_parent;
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