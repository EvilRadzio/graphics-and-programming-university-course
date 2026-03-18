#pragma once

#include "TextureManager.hpp"

namespace sf
{
	class Texture;
}

namespace px
{
	class AssetManager
	{
	public:

		void add(sf::Texture&& texture, const std::string& name);
		const sf::Texture& texture(const std::string& name) const;
		const sf::Texture& texture(const TextureHandle handle) const;
		TextureHandle textureHandle(const std::string& name) const;

	private:

		TextureManager m_textures;
	};
}