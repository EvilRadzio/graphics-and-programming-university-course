#pragma once

#include "Game/Types.hpp"

namespace Scenes
{
	class Pause : public Scene
	{
	public:

		Pause(ApiScene& api, Context& ctx) : Scene(api, ctx)
		{
			properties.renderThrough = true;
		}

		void update(px::ApiUpdate& api) override
		{
			if (scene.input.isPressed(sf::Keyboard::Scancode::Escape))
			{
				scene.comms.pop({});
			}
		}

		void draw(px::ApiDraw& api) const override
		{
			sf::RectangleShape darkRect(static_cast<sf::Vector2f>(api.window.getSize()));
			darkRect.setFillColor(sf::Color(0, 0, 0, 100));

			api.window.draw(darkRect);

			sf::Text text(api.assets.font, "Pause", 72);
			text.setPosition({ 100, 100 });
			
			api.window.draw(text);
		}
	};
}