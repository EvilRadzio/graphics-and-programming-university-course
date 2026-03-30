#pragma once

#include <SFML/Graphics.hpp>

#include "pXL/Registry.hpp"
#include "TileSprite.hpp"
#include "SpriteDefinition.hpp"
#include "Background.hpp"

namespace px
{
	struct Assets
	{
		Registry<sf::Texture> textures{};
		Registry<TileSprite> tileSprites{};
		Registry<SpriteDefinition> entitySprites{};
		Registry<BackgroundData> backgrounds{};
		sf::Font font{};
	};
}