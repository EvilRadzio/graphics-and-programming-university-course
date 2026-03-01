#pragma once

namespace sf
{
	class RenderWindow;
}

namespace Engine
{
	class TextureManager;

	struct DrawApi
	{
		sf::RenderWindow& window;
		const TextureManager& textures;
	};
}