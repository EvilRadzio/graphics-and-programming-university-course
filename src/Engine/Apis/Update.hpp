#pragma once

#include <SFML/System.hpp>

namespace sf
{
	class Window;
}

namespace Engine::Apis
{
	struct Update
	{
		const sf::Window& window;
		const sf::Time dt;
	};
}