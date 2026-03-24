#pragma once

#include <iostream>

#include "pXL/pXL.hpp"

#include "Types.hpp"
#include "Scenes/Scenes.hpp"

class Game : public px::Client<Internal>
{
public:

	Game()
	{
		px::Load::recursive("resources/textures", [&](const auto& path, const auto& name) {
			sf::Texture texture;
			if (!texture.loadFromFile(path))
			{
				return;
			}

			assets.textures.set(std::move(texture), name);

			std::cout << "Loaded: " << name << std::endl;
		});

		m_scenes.registerScene(SceneId::MainMenu, [&]() { return std::make_unique<Scenes::MainMenu>(buildSceneApi(), m_context); });
		m_scenes.registerScene(SceneId::TicTacToe, [&]() { return std::make_unique<Scenes::TicTacToe>(buildSceneApi(), m_context); });
		m_scenes.registerScene(SceneId::LevelEditor, [&]() { return std::make_unique<Scenes::LevelEditor>(buildSceneApi(), m_context); });
		m_scenes.registerScene(SceneId::Platforming, [&]() { return std::make_unique<Scenes::Platforming>(buildSceneApi(), m_context); });
		m_scenes.pushScene(SceneId::MainMenu);

		m_context.tiles["empty"] = Tile{Tile::Type::Air, "", "empty"};
		m_context.tiles["solid_block"] = Tile{ Tile::Type::Solid, "solid_block", "solid_block"};

		assets.font = sf::Font("resources/Butterpop.otf");
	}
};