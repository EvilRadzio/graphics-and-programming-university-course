#pragma once

#include <string>
#include <cstdint>

#include <SFML/Graphics.hpp>

#include "pXL/Manager.hpp"

namespace px
{
	class TileSprite
	{
		std::string textureName;

		sf::Sprite get(uint8_t adjesant, Manager<sf::Texture> textures) const
		{
			const sf::Texture& texture = textures.get(textureName);
			sf::Sprite out(texture);
			sf::IntRect rect({0,0}, static_cast<sf::Vector2i>(texture.getSize()) / 4);
			const int32_t side = rect.size.x;
			switch (adjesant)
			{
			case 15:
				rect.position.x = 1 * side;
				rect.position.y = 1 * side;
				break;
			case 14:
				rect.position.x = 1 * side;
				rect.position.y = 0 * side;
				break;
			case 13:
				rect.position.x = 2 * side;
				rect.position.y = 1 * side;
				break;
			case 12:
				rect.position.x = 2 * side;
				rect.position.y = 0 * side;
				break;
			case 11:
				rect.position.x = 1 * side;
				rect.position.y = 2 * side;
				break;
			case 10:
				rect.position.x = 1 * side;
				rect.position.y = 3 * side;
				break;
			case 9:
				rect.position.x = 2 * side;
				rect.position.y = 2 * side;
				break;
			case 8:
				rect.position.x = 2 * side;
				rect.position.y = 3 * side;
				break;
			case 7:
				rect.position.x = 0 * side;
				rect.position.y = 1 * side;
				break;
			case 6:
				rect.position.x = 0 * side;
				rect.position.y = 0 * side;
				break;
			case 5:
				rect.position.x = 3 * side;
				rect.position.y = 1 * side;
				break;
			case 4:
				rect.position.x = 3 * side;
				rect.position.y = 0 * side;
				break;
			case 3:
				rect.position.x = 0 * side;
				rect.position.y = 2 * side;
				break;
			case 2:
				rect.position.x = 0 * side;
				rect.position.y = 3 * side;
				break;
			case 1:
				rect.position.x = 3 * side;
				rect.position.y = 2 * side;
				break;
			case 0:
				rect.position.x = 3 * side;
				rect.position.y = 3 * side;
				break;
			}

			out.setTextureRect(rect);
			return out;
		}
	};
}