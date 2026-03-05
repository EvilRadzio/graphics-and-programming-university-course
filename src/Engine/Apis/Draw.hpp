#pragma once

namespace sf
{
	class RenderTarget;
	class Font;
}

namespace Engine::Resources::Textures
{
	class Manager;
}

namespace Engine::World::Tiles
{
	class Manager;
	class TextureManager;
}

namespace Engine::Apis
{
	struct Draw
	{
		sf::RenderTarget& window;
		const Resources::Textures::Manager& textures;
		const World::Tiles::Manager& tiles;
		const World::Tiles::TextureManager& tileTextures;
		const sf::Font& font;
	};
}