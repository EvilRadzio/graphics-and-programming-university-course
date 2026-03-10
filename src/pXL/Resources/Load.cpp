#include <iostream>

#include "Load.hpp"

#include "TextureManager.hpp"
#include "TileTextureManager.hpp"

void px::Load::texturesRecursive(px::TextureManager& textures, const std::string& directoryPath)
{
	if (!std::filesystem::exists(directoryPath))
	{
		return;
	}

	for (const auto& entry : std::filesystem::recursive_directory_iterator(directoryPath))
	{
		if (!entry.is_regular_file())
		{
			continue;
		}

		sf::Texture texture;
		if (!texture.loadFromFile(entry.path()))
		{
			continue;
		}

		std::string path = std::filesystem::relative(entry.path(), directoryPath).replace_extension().generic_string();

		textures.add(std::move(texture), path);

		std::cout << "Loaded: " << path << std::endl;
	}
}

void px::Load::tileTextures(px::TileTextureManager& tileTextures, std::vector<std::pair<px::TileHandle, std::string>>&& loadedTiles)
{
	for (auto& [handle, name] : loadedTiles)
	{
		tileTextures.setTileTexture(handle, name);
	}
}