

#include "AssetManager.hpp"

void px::AssetManager::loadTexture(sf::Texture&& texture, const std::string& name)
{
	m_textures.add(std::move(texture), name);
}

sf::Texture px::AssetManager::getTexture(const std::string& name)
{
	return m_textures.texture(name);
}