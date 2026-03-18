

#include "AssetManager.hpp"

void px::AssetManager::add(sf::Texture&& texture, const std::string& name)
{
	m_textures.add(std::move(texture), name);
}

const sf::Texture& px::AssetManager::texture(const std::string& name) const
{
	return m_textures.texture(name);
}

const sf::Texture& px::AssetManager::texture(const TextureHandle handle) const
{
	return m_textures.texture(handle);
}

px::TextureHandle px::AssetManager::textureHandle(const std::string & name) const
{
	return m_textures.handle(name);
}