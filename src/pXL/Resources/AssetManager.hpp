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

		void loadTexture(sf::Texture&& texture, const std::string& name);
		sf::Texture getTexture(const std::string& name);

	private:

		TextureManager m_textures;
	};
}