#pragma once

#include <SFML/System.hpp>

namespace Engine
{
	class Input;

	struct UpdateApi
	{
		const Input& input;
		const sf::Time dt;
	};
}