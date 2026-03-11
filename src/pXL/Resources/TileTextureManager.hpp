#pragma once

#include <unordered_map>
#include <string>

#include "TextureHandle.hpp"
#include "pXL/World/TileManager.hpp"

namespace px
{
	class TileTextureManager
	{
	public:

		TileTextureManager(const TextureManager& textureManager)
			: m_textureManager(textureManager) {}

		~TileTextureManager() = default;

		void setTileTexture(const TileHandle tileHandle, const std::string& texturePath)
		{
			m_tileTextures.insert(std::pair{
				tileHandle.id,
				m_textureManager.handle(texturePath)
			});
		}

		bool hasTexture(const TileHandle tileHandle) const
		{
			return m_tileTextures.count(tileHandle.id);
		}

		TextureHandle handle(const TileHandle tileHandle) const
		{
			return m_tileTextures.at(tileHandle.id);
		}

	private:

		std::unordered_map<size_t, TextureHandle> m_tileTextures;
		const TextureManager& m_textureManager;
	};
}