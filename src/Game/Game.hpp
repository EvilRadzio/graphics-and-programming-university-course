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
			registerScene(Schema::SceneId::MainMenu, []() {return std::make_unique<Scenes::MainMenu>(); });
			registerScene(Schema::SceneId::LevelEditor, []() {return std::make_unique<Scenes::LevelEditor>(); });
			setStartingScene(Schema::SceneId::MainMenu);
		}
	};
}