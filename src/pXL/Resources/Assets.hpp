#pragma once

#include <SFML/Graphics.hpp>

#include "pXL/Registry.hpp"
#include "TileSprite.hpp"
#include "Background.hpp"
#include "Sprite.hpp"

namespace px
{
	struct Assets
	{
		Registry<sf::Texture> textures{};
		Registry<TileSprite> tileSprites{};
		Registry<BackgroundData> backgrounds{};
		Registry<AnimationClipMap> clipMaps{};
		sf::Font font{};
	};
}