#pragma once

#include <SFML/System.hpp>

namespace sf
{
	class Window;
}

namespace px
{
	class Transition;

	struct ApiUpdate
	{
		const sf::Window& window;
		const sf::Time dt;
		Transition& transition;
	};
}