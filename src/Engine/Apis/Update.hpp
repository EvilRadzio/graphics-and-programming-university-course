#pragma once

#include <SFML/System.hpp>

namespace Engine
{
	class Input;
}

namespace Engine::Apis
{
	struct Update
	{
		const Input& input;
		const sf::Time dt;
	};
}