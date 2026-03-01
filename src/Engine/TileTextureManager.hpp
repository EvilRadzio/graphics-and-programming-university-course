#pragma once

#include <unordered_map>

#include "TileHandle.hpp"
#include "TextureHandle.hpp"

// for now it's very ugly but works

namespace Engine
{
	class TileTextureManager
	{
	public:

		TileTextureManager() = default;
		~TileTextureManager() = default;

		void setTileTexture(const TileHandle tileHandle, const TextureHandle textureHandle)
		{
			m_tileTextures.insert(std::pair{ tileHandle.id, textureHandle });
		}

		TextureHandle getTexture(const TileHandle tileHandle) const
		{
			return m_tileTextures.at(tileHandle.id);
		}

	private:

		std::unordered_map<size_t, TextureHandle> m_tileTextures;
	};
}