#pragma once

namespace sf
{
	class RenderTarget;
	class Font;
}

namespace Engine
{
	class TextureManager;
	class TileManager;
	class TileTextureManager;
	class Input;
}

namespace Engine::Apis
{
	struct Draw
	{
		sf::RenderTarget& window;
		const TextureManager& textures;
		const TileManager& tiles;
		const TileTextureManager& tileTextures;
		const Input& input;
		const sf::Font& font;
	};
}