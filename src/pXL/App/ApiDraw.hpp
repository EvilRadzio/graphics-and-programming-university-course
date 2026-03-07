#pragma once

namespace sf
{
	class RenderTarget;
	class Font;
}

namespace px
{
	class TextureManager;
	class TileManager;
	class TileTextureManager;

	struct ApiDraw
	{
		sf::RenderTarget& window;
		const TextureManager& textures;
		const TileManager& tiles;
		const TileTextureManager& tileTextures;
		const sf::Font& font;
	};
}