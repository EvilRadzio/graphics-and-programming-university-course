#pragma once

#include <vector>
#include <stdint.h>
#include <cassert>

#include <SFML/System.hpp>

#include "Manager.hpp"

namespace Engine::Tiles
{
	class Map
	{
	public:

		Map(const Manager& tileManager, size_t width, size_t height) :
			m_tiles(width* height, tileManager.getEmptyHandle()),
			m_tileManager(tileManager),
			m_width(width),
			m_height(height) {}

		Handle atHandle(sf::Vector2u position)
		{
			assert(position.x <= m_width && position.y <= m_height);

			return m_tiles[position.y * m_width + position.x];
		}

		const Tile& atTile(sf::Vector2u position)
		{
			assert(position.x <= m_width && position.y <= m_height);

			return m_tileManager.get(m_tiles[position.y * m_width + position.x]);
		}

		void set(sf::Vector2u position, Handle tileHandle)
		{
			assert(position.x <= m_width && position.y <= m_height);

			m_tiles[position.y * m_width + position.x] = tileHandle;
		}

	private:

		std::vector<Handle> m_tiles;
		const Manager& m_tileManager;
		size_t m_width;
		size_t m_height;
	};
}