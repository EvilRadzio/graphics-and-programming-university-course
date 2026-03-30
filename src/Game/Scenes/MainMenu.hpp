#pragma once

#include "Game/Types.hpp"

namespace Scenes
{
	class MainMenu : public Scene
	{
	public:

		MainMenu(ApiScene& api, Context& ctx) : Scene(api, ctx), m_menu({360, 360})
		{
			m_menu.addButton("Play", [&]() {scene.comms.push(SceneId::Platforming, {}); });
			m_menu.addButton("Level Editor", [&]() {scene.comms.push(SceneId::LevelEditor, {}); });
			m_menu.addButton("Exit", [&]() {});
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
	};
}