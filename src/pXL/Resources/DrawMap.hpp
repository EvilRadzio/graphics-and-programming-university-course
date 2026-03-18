#pragma once

#include "../World/Map.hpp"
#include "TileTextureManager.hpp"
#include "TextureManager.hpp"

namespace px
{
	class DrawMap
	{
	public:

		DrawMap(const Map& map, const TileTextureManager& tileTextures, const TextureManager& textures) :
			m_map(map),
			m_tileTextures(tileTextures),
			m_textures(textures) {}

		sf::Vector2u size() const { return m_map.size(); }

		bool hasTexture(sf::Vector2u position) const
		{
			return m_tileTextures.hasTexture(m_map.at(position));
		}

		const sf::Texture& at(sf::Vector2u position) const
		{
			return m_textures.texture(m_tileTextures.handle(m_map.at(position)));
		}

	private:

		const Map& m_map;
		const TileTextureManager& m_tileTextures;
		const TextureManager& m_textures;
	};
}