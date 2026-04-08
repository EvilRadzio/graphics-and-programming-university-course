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
		recursiveLoad("resources/textures", [&](const auto& path, const auto& name) {
			sf::Texture texture;
			if (!texture.loadFromFile(path))
			{
				return;
			}
			texture.setRepeated(true);

			assets.textures.set(name, std::move(texture));

			std::cout << "Loaded: " << name << std::endl;
		});

		scenes.registerScene(SceneId::MainMenu, [&]() { return std::make_unique<Scenes::MainMenu>(apiScene, ctx); });
		scenes.registerScene(SceneId::LevelEditor, [&]() { return std::make_unique<Scenes::LevelEditor>(apiScene, ctx); });
		scenes.registerScene(SceneId::Platforming, [&]() { return std::make_unique<Scenes::Platforming>(apiScene, ctx); });
		scenes.registerScene(SceneId::Pause, [&]() {return std::make_unique<Scenes::Pause>(apiScene, ctx); });
		scenes.push(SceneId::MainMenu);

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

		px::Clip idleClip(std::move(idle));
		idleClip.setLooping(true);
		px::Clip runClip(std::move(run));
		runClip.setLooping(true);

		px::SpriteData animations(assets.textures.get("knight"));
		animations.setClip("idle", std::move(idleClip));
		animations.setClip("run", std::move(runClip));

		assets.sprites.set("knight", std::move(animations));

		px::BackgroundData background(
			{
				{ assets.textures.get("background/0"), 0.32768f },
				{ assets.textures.get("background/1"), 0.4096f },
				{ assets.textures.get("background/2"), 0.512f },
				{ assets.textures.get("background/3"), 0.64f },
				{ assets.textures.get("background/4"), 0.8f },
				{ assets.textures.get("background/5"), 1.0f }
			}
		);

		assets.backgrounds.set("background", std::move(background));

		assets.font = sf::Font("resources/Butterpop.otf");
	}
};