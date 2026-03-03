#pragma once

#include "Game/Schema/Types.hpp"

namespace Game::Scenes
{
	class MainMenu : public Schema::Scene
	{
	public:

		void updateImgui(Schema::Context& context, Engine::UpdateApi& api) override
		{
			if (ImGui::Begin("Miku settings!"))
			{
				ImGui::Checkbox("Visible miku!!!", &m_showMiku);
			}
			ImGui::End();
		}

		void update(Schema::Context& context, Engine::UpdateApi& api) override
		{
			if (api.input.isPressed(sf::Keyboard::Scancode::Space))
			{
				pushScene(Schema::SceneId::TicTacToe);
			}
		}

		void draw(const Schema::Context& context, Engine::DrawApi& api) const override
		{
			api.window.clear(sf::Color::Blue);

			sf::Vector2f windowSize = static_cast<sf::Vector2f>(api.window.getSize());
			
			if (m_showMiku)
			{
				sf::RectangleShape mikuShape(windowSize * 0.8f);
				mikuShape.move(sf::Vector2f(windowSize.x / 10.0f, windowSize.y / 20.0f));
				mikuShape.setTexture(&api.textures.getTexture("player"));

				api.window.draw(mikuShape);
			}

			sf::Text text(api.font, "Click space to continue..");
			text.setCharacterSize(40);
			text.setOrigin(static_cast<sf::Vector2f>(text.getLocalBounds().size) / 2.0f);
			text.setPosition(sf::Vector2f(windowSize.x / 2.0f, (windowSize.y / 10) * 9));

			api.window.draw(text);
		}

	private:

		bool m_showMiku{ true };
	};
}