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

		scenes.registerScene(SceneId::MainMenu, [&]() { return std::make_unique<Scenes::MainMenu>(apiScene, ctx); });
		scenes.registerScene(SceneId::TicTacToe, [&]() { return std::make_unique<Scenes::TicTacToe>(apiScene, ctx); });
		scenes.registerScene(SceneId::LevelEditor, [&]() { return std::make_unique<Scenes::LevelEditor>(apiScene, ctx); });
		scenes.registerScene(SceneId::Platforming, [&]() { return std::make_unique<Scenes::Platforming>(apiScene, ctx); });
		scenes.registerScene(SceneId::Pause, [&]() {return std::make_unique<Scenes::Pause>(apiScene, ctx); });
		scenes.pushScene(SceneId::MainMenu);

		ctx.tiles["empty"] = Tile{Tile::Type::Air, "", "empty"};
		ctx.tiles["solid_block"] = Tile{ Tile::Type::Solid, "solid_block", "solid_block"};

		assets.tileSprites.set(px::TileSprite{"tiles/moss_on_cobble_tileset"}, "solid_block");

		assets.font = sf::Font("resources/Butterpop.otf");
	}
};