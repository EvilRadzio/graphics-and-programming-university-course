#pragma once

#include "../World/IdMap.hpp"
#include "TileTextureManager.hpp"
#include "AssetManager.hpp"

namespace px
{
	class DrawMap
	{
	public:

		DrawMap(const IdMap& map, const TileTextureManager& tileTextures, const AssetManager& assets) :
			m_map(map),
			m_tileTextures(tileTextures),
			m_assets(assets) {}

		sf::Vector2u size() const { return m_map.size(); }

		bool hasTexture(sf::Vector2u position) const
		{
			auto handle = m_map.at(position);
			if (!handle) return false;

			return m_tileTextures.hasTexture(m_map.at(position).value());
		}

		const sf::Texture& at(sf::Vector2u position) const
		{
			return m_assets.texture(m_tileTextures.handle(m_map.at(position).value()));
		}

	private:

		const IdMap& m_map;
		const TileTextureManager& m_tileTextures;
		const AssetManager& m_assets;
	};
}