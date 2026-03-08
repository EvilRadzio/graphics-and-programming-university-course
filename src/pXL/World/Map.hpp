#pragma once

#include <vector>
#include <stdint.h>
#include <cassert>

#include <SFML/System.hpp>

#include "TileManager.hpp"

namespace px
{
	class Map
	{
	public:

		Map(TileHandle baseTile, size_t width, size_t height) :
			m_tiles(width* height, baseTile),
			m_width(width),
			m_height(height) {}

		size_t width() const { return m_width; }
		size_t height() const { return m_height; }

		TileHandle at(sf::Vector2u position) const
		{
			assert(position.x <= m_width && position.y <= m_height);

			return m_tiles[position.y * m_width + position.x];
		}

		void set(sf::Vector2u position, TileHandle tileHandle)
		{
			assert(position.x <= m_width && position.y <= m_height);

			m_tiles[position.y * m_width + position.x] = tileHandle;
		}

	private:

		std::vector<TileHandle> m_tiles;
		size_t m_width;
		size_t m_height;
	};
}