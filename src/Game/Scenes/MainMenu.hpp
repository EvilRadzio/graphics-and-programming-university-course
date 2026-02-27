#pragma once

#include "Game/Schema/Types.hpp"

namespace Game::Scenes
{
	class MainMenu : public Schema::Scene
	{
	public:

		MainMenu() = default;
		~MainMenu() = default;

		void update(Schema::Context& context, Engine::UpdateApi& api) override
		{
			if (api.input.isAnyPressed())
			{
				pushScene(Schema::SceneId::LevelEditor);
			}
		}

		void draw(const Schema::Context& context, Engine::DrawApi& api) const override
		{
			api.window.clear(sf::Color::Blue);
		}
	};
}