#pragma once

#include "pXL/pXL.hpp"

#include "Types.hpp"
#include "Scenes/Scenes.hpp"

class Game : public px::Client<Internal>
{
public:

	Game()
	{
		m_textures.loadAll("resources/textures");

		m_scenes.registerScene(SceneId::MainMenu, [&]() {return std::make_unique<Scenes::MainMenu>(buildSceneApi()); });
		m_scenes.registerScene(SceneId::TicTacToe, [&]() {return std::make_unique<Scenes::TicTacToe>(buildSceneApi()); });
		m_scenes.registerScene(SceneId::LevelEditor, [&]() { return std::make_unique<Scenes::LevelEditor>(buildSceneApi()); });
		m_scenes.pushScene(SceneId::MainMenu);

		px::Tile someTile(px::TileType::solid);
		m_tileTextures.setTileTexture(m_tiles.add(someTile), "solid_block.png");

		m_font = sf::Font("resources/Butterpop.otf");
	}
};