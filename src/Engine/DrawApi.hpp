#pragma once

namespace sf
{
	class RenderWindow;
	class Font;
}

namespace Engine
{
	class TextureManager;
	class TileManager;
	class TileTextureManager;
	class Input;

	struct DrawApi
	{
		sf::RenderWindow& window;
		const TextureManager& textures;
		const TileManager& tiles;
		const TileTextureManager& tileTextures;
		const Input& input;
		const sf::Font& font;
	};
}