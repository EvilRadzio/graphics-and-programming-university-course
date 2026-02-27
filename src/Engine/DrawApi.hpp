#pragma once

namespace sf
{
	class RenderWindow;
}

namespace Engine
{
	class ResourceManager;

	struct DrawApi
	{
		sf::RenderWindow& window;
		const ResourceManager& resources;
	};
}