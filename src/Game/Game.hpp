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

			assets.textures.set(name, std::move(texture));

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

		assets.tileSprites.set("solid_block", px::TileSprite{ "tiles/moss_on_cobble_tileset" });

		std::vector<px::Frame> idle;
		for (int32_t x = 0; x < 4; ++x)
		{
			idle.push_back({ sf::IntRect({x * 32, 0}, {32, 32}), sf::milliseconds(200) });
		}

		std::vector<px::Frame> run;
		for (int32_t y = 2; y < 4; ++y)
		{
			for (int32_t x = 0; x < 8; ++x)
			{
				run.push_back({ sf::IntRect({x * 32, y * 32}, {32, 32}), sf::milliseconds(125) });
			}
		}

		std::unordered_map<std::string, px::Clip> clips{
			{"idle", px::Clip(true, std::move(idle))},
			{"run", px::Clip(true, std::move(run))}
		};

		px::SpriteDefinition sprite{ std::move(clips), "knight", sf::IntRect({-72, -90}, {144, 144}) };

		assets.entitySprites.set("knight", std::move(sprite));

		assets.font = sf::Font("resources/Butterpop.otf");
	}
};