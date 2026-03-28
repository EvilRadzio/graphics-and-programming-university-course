#pragma once

#include <string>

#include <SFML/System.hpp>
#include <SFML/Graphics/Rect.hpp>

namespace px
{
	struct SpriteInstance
	{
		std::string spriteName;
		std::string current;
		sf::Time elapsed{ sf::Time::Zero };
	};
}