#pragma once

#include <SFML/System.hpp>

namespace sf
{
	class Window;
}

namespace px
{
	struct ApiUpdate
	{
		const sf::Window& window;
		const sf::Time dt;
	};
}