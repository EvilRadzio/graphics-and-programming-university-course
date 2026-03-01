#pragma once

#include "Game/Schema/Types.hpp"

namespace Game::Scenes
{
	class MainMenu : public Schema::Scene
	{
	public:

		void update(Schema::Context& context, Engine::UpdateApi& api) override
		{
			if (api.input.isAnyPressed())
			{
				pushScene(Schema::SceneId::TicTacToe);
			}
		}

		void draw(const Schema::Context& context, Engine::DrawApi& api) const override
		{
			api.window.clear(sf::Color::Blue);

			sf::Text text(api.font, "Click any button to continue..");
			text.setCharacterSize(40);
			text.setOrigin(static_cast<sf::Vector2f>(text.getLocalBounds().size) / 2.0f);
			text.setPosition(static_cast<sf::Vector2f>(api.window.getSize()) / 2.0f);

			api.window.draw(text);
		}
	};
}