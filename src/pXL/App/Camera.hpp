#pragma once

#include <stdint.h>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

namespace px
{
	struct Camera
	{
		sf::Vector2f position{ 0.0f, 0.0f };

		sf::View view(const sf::RenderTarget& target, uint32_t tileSize) const
		{
			return sf::View(
				position * static_cast<float>(tileSize),
				static_cast<sf::Vector2f>(target.getSize())
			);
		}
	};
}