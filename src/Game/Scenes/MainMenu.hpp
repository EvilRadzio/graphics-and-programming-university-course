#pragma once

#include "Game/Types.hpp"

namespace Scenes
{
	class MainMenu : public Scene
	{
	public:

		MainMenu(ApiScene& api, Context& ctx) : Scene(api, ctx), m_menu({360, 360})
		{
			m_menu.addButton("Play", [&]() { api.comms.fadeInOut();  m_action = Action::Play; });
			m_menu.addButton("Level Editor", [&]() { api.comms.fadeInOut();  m_action = Action::LevelEditor; });
			m_menu.addButton("Exit", [&]() {});
		}

		void update(px::ApiUpdate& api) override
		{
			if (scene.comms.isMidFadeInOut())
			{
				if (m_action == Action::Play)
				{
					scene.comms.push(SceneId::Platforming);
				}
				else if (m_action == Action::LevelEditor)
				{
					scene.comms.push(SceneId::LevelEditor);
				}
			}

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

		enum class Action { Play, LevelEditor };
		Action m_action;
		px::TextMenu m_menu;
	};
}