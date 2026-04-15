#pragma once

#include "pXL/pXL.hpp"

namespace Scenes
{
	class MainMenu : public px::Scene
	{
	public:

		MainMenu(px::ApiScene& api, sf::Window& window) :
			Scene(api),
			m_menu({360, 360}),
			m_window(window)
		{
			m_menu.addButton("Play", [&]() { api.transition.start([&]() { scene.comms.push("Platforming"); }); });
			m_menu.addButton("Level Editor", [&]() { api.transition.start([&]() { scene.comms.push("LevelEditor"); }); });
			m_menu.addButton("Exit", [&]() { window.close(); });
		}

		void update(px::ApiUpdate& api) override
		{
			if (scene.input.isPressed(sf::Keyboard::Scancode::W))
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
			api.window.clear(sf::Color(0x222222ff));

			sf::RectangleShape mikuShape(static_cast<sf::Vector2f>(api.window.getSize()));
			mikuShape.setTexture(&api.assets.textures.get("player"));

			api.window.draw(mikuShape);

			m_menu.draw(api);
		}

	private:

		px::TextMenu m_menu;
		sf::Window& m_window;
	};
}