#pragma once

#include "Game/Types.hpp"

namespace Scenes
{
	class Pause : public Scene
	{
	public:

		Pause(ApiScene& api, Context& ctx) : Scene(api, ctx), m_menu({360, 260})
		{
			api.properties.setTransparency(true);

			m_menu.addButton("Resume", [&]() {scene.comms.pop({}); });
			m_menu.addButton("Exit", [&]() {scene.comms.popUntil(SceneId::MainMenu, {}); });
		}

		void update(px::ApiUpdate& api) override
		{
			if (scene.input.isPressed(sf::Keyboard::Scancode::Escape))
			{
				scene.comms.pop({});
			}
			else if (scene.input.isPressed(sf::Keyboard::Scancode::W))
			{
				m_menu.moveUp();
			}
			else if (scene.input.isPressed(sf::Keyboard::Scancode::S))
			{
				m_menu.moveDown();
			}
			else if (scene.input.isPressed(sf::Keyboard::Scancode::Space))
			{
				m_menu.activate();
			}
		}

		void draw(px::ApiDraw& api) const override
		{
			sf::RectangleShape darkRect(static_cast<sf::Vector2f>(api.window.getSize()));
			darkRect.setFillColor(sf::Color(0, 0, 0, 100));

			api.window.draw(darkRect);

			sf::Text text(api.assets.font, "Pause", 72);
			
			api.window.draw(text);

			m_menu.draw(api);
		}

	private:

		px::TextMenu m_menu;
	};
}