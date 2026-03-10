#pragma once

#include <vector>

#include "../World/TileHandle.hpp"

namespace px
{
	class TextureManager;
	class TileTextureManager;
}

namespace px::Load
{
	void texturesRecursive(px::TextureManager& textures, const std::string& directoryPath, bool smooth = false);
	void tileTextures(px::TileTextureManager& tileTextures, std::vector<std::pair<TileHandle, std::string>>&& loadedTiles);
}