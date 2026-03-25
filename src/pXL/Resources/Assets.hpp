#pragma once

#include <SFML/Graphics.hpp>

#include "pXL/Manager.hpp"
#include "TileSprite.hpp"

namespace px
{
	struct Assets
	{
		Manager<sf::Texture> textures;
		Manager<TileSprite> tileSprites;
		sf::Font font;
	};
}