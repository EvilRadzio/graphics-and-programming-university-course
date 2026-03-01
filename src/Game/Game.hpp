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

			m_scenes.registerScene(Schema::SceneId::MainMenu, []() {return std::make_unique<Scenes::MainMenu>(); });
			m_scenes.registerScene(Schema::SceneId::LevelEditor, []() {return std::make_unique<Scenes::LevelEditor>(); });
			m_scenes.pushScene(Schema::SceneId::MainMenu);
		}
	};
}