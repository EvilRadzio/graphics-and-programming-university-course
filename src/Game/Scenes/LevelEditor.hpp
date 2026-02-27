#pragma once

#include "Game/Schema/Types.hpp"

namespace Game::Scenes
{
	class LevelEditor : public Schema::Scene
	{
	public:

		LevelEditor() = default;
		~LevelEditor() = default;

		void update(Schema::Context& context, Engine::UpdateApi& api) override
		{
			if (api.input.isPressed(sf::Keyboard::Scan::Escape))
			{
				popScene();
			}
		}

		void draw(const Schema::Context& context, Engine::DrawApi& api) const override
		{
			api.window.clear(sf::Color::Cyan);
		}
	};
}