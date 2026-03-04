#pragma once

namespace sf
{
	class RenderWindow;
}

namespace Engine
{
	class TileManager;
}

namespace Engine::Apis
{
	struct UpdateGui
	{
		sf::RenderWindow& window;
		const TileManager& tiles;
	};
}