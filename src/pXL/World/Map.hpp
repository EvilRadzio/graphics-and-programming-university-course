#pragma once

#include <vector>
#include <stdint.h>
#include <cassert>
#include <optional>

#include <SFML/System.hpp>

#include "TileManager.hpp"

namespace px
{
	class Map
	{
	public:

		Map(sf::Vector2u size) :
			m_tiles(size.x* size.y, {}),
			m_size(size) {}

		sf::Vector2u size() const { return m_size; }

		std::optional<TileHandle> at(sf::Vector2u position) const
		{
			assert(position.x <= m_size.x && position.y <= m_size.y);

			return m_tiles[position.y * m_size.x + position.x];
		}

		void set(sf::Vector2u position, TileHandle tileHandle)
		{
			assert(position.x <= m_size.x && position.y <= m_size.y);

			m_tiles[position.y * m_size.x + position.x] = tileHandle;
		}

	private:

		std::vector<std::optional<TileHandle>> m_tiles;
		sf::Vector2u m_size;
	};
}