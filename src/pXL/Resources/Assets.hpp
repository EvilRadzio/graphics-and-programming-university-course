#pragma once

#include <SFML/Graphics.hpp>

#include "pXL/Manager.hpp"

namespace px
{
	struct Assets
	{
		Manager<sf::Texture> textures;
		sf::Font font;
	};
}