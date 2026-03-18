#pragma once

namespace sf
{
	class RenderTarget;
	class Font;
}

namespace px
{
	class AssetManager;
	class TileManager;
	class TileTextureManager;

	struct ApiDraw
	{
		sf::RenderTarget& window;
		const AssetManager& assets;
		const TileManager& tiles;
		const TileTextureManager& tileTextures;
		const sf::Font& font;
	};
}