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

		size_t width() const { return m_map.width(); }
		size_t height() const { return m_map.height(); }

		sf::Texture at(sf::Vector2u position) const
		{
			return m_textures.texture(m_tileTextures.handle(m_map.at(position)));
		}

	private:

		const Map& m_map;
		const TileTextureManager& m_tileTextures;
		const TextureManager& m_textures;
	};
}