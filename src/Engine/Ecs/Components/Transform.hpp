#pragma once

#include <SFML/System.hpp>

namespace Engine::Ecs::Components
{
	struct Transform
	{
		sf::Vector2f position;
		sf::Vector2f velocity;
	};
}