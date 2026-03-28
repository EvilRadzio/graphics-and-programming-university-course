#pragma once

#include <SFML/Graphics.hpp>

namespace px
{
	struct Frame
	{
		sf::IntRect rect;
		sf::Time time;
	};
}