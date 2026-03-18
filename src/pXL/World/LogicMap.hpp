#pragma once

#include "Map.hpp"
#include "TileManager.hpp"

namespace px
{
	class LogicMap
	{
	public:

		LogicMap(const Map& map, const TileManager& tiles) :
			m_map(map),
			m_tiles(tiles) {}

		sf::Vector2u size() const { return m_map.size(); }

		bool isTile(sf::Vector2u position) const
		{
			return m_map.at(position).has_value();
		}

		const Tile& at(sf::Vector2u position) const
		{
			return m_tiles.tile(m_map.at(position).value());
		}

	private:

		const Map& m_map;
		const TileManager& m_tiles;
	};
}