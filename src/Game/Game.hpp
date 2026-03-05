#pragma once

#include "Schema/Types.hpp"
#include "Scenes/Scenes.hpp"

namespace Game
{
	class Game : public Engine::Game<Schema::Internal>
	{
	public:

		Game()
		{
			m_textures.loadAll("resources/textures");

			m_scenes.registerScene(Schema::SceneId::MainMenu, [&]() {return std::make_unique<Scenes::MainMenu>(buildSceneApi()); });
			m_scenes.registerScene(Schema::SceneId::TicTacToe, [&]() {return std::make_unique<Scenes::TicTacToe>(buildSceneApi()); });
			m_scenes.registerScene(Schema::SceneId::LevelEditor, [&]() { return std::make_unique<Scenes::LevelEditor>(buildSceneApi()); });
			m_scenes.pushScene(Schema::SceneId::MainMenu);

			Engine::World::Tiles::Tile someTile(Engine::World::Tiles::TileType::solid);
			m_tileTextures.setTileTexture(m_tiles.add(someTile), "solid_block.png");

			m_font = sf::Font("resources/Butterpop.otf");
		}
	};
}