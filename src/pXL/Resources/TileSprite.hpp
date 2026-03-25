#pragma once

#include <string>
#include <cstdint>

#include <SFML/Graphics.hpp>

#include "pXL/Manager.hpp"

namespace px
{
	struct TileSprite
	{
		std::string textureName;

		sf::Sprite get(uint8_t adjacent, const Manager<sf::Texture>& textures) const
		{
            const sf::Texture& texture = textures.get(textureName);
            sf::Sprite out(texture);

            sf::IntRect rect = out.getTextureRect();
            rect.size /= 4;

            const int32_t side = rect.size.x;

            switch (adjacent)
            {
            case 15: rect.position = { 1 * side, 1 * side }; break;
            case 14: rect.position = { 1 * side, 0 * side }; break;
            case 13: rect.position = { 2 * side, 1 * side }; break;
            case 12: rect.position = { 2 * side, 0 * side }; break;
            case 11: rect.position = { 1 * side, 2 * side }; break;
            case 10: rect.position = { 1 * side, 3 * side }; break;
            case 9:  rect.position = { 2 * side, 2 * side }; break;
            case 8:  rect.position = { 2 * side, 3 * side }; break;
            case 7:  rect.position = { 0 * side, 1 * side }; break;
            case 6:  rect.position = { 0 * side, 0 * side }; break;
            case 5:  rect.position = { 3 * side, 1 * side }; break;
            case 4:  rect.position = { 3 * side, 0 * side }; break;
            case 3:  rect.position = { 0 * side, 2 * side }; break;
            case 2:  rect.position = { 0 * side, 3 * side }; break;
            case 1:  rect.position = { 3 * side, 2 * side }; break;
            case 0:  rect.position = { 3 * side, 3 * side }; break;
            }

            out.setTextureRect(rect);
            return out;
		}
	};
}