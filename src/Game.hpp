#pragma once

#include <stdint.h>
#include <algorithm>

#include <SFML/Graphics.hpp>

#include "SceneManager.hpp"

namespace gp
{
	template <typename SceneTagEnum, typename ContextObject>
	class Game
	{
	public:

		Game() : m_window(sf::VideoMode(sf::Vector2u{ 720,720 }), "Platformer") {};
		virtual ~Game() = default;

		void run()
		{
			m_window.setFramerateLimit(60);

			m_clock.restart();

			while (m_window.isOpen())
			{
				m_input.newFrame();

				while (const auto event = m_window.pollEvent())
				{
					m_input.readEvent(*event);

					if (event->is<sf::Event::Closed>())
					{
						m_window.close();
					}
				}

				m_accumulated += m_clock.restart();
				m_accumulated = std::min(m_accumulated, k_maxAccumulated);

				while (m_accumulated >= k_fixedDt)
				{
					m_accumulated -= k_fixedDt;

					m_scenes.update(m_context, m_input);

					m_input.newFrame();
				}

				m_window.clear(sf::Color::Black);

				m_scenes.draw(m_context, m_window);

				m_window.display();
			}
		}

	protected:

		void registerScene(SceneTagEnum tag, SceneManager<SceneTagEnum, ContextObject>::SceneFactory&& factoryFunction)
		{
			m_scenes.registerScene(tag, std::move(factoryFunction));
		}

		void setStartingScene(SceneTagEnum tag)
		{
			assert(m_scenes.empty());

			m_scenes.pushScene(tag);
		}

	private:

		static constexpr uint32_t k_tps{ 60 };
		static constexpr sf::Time k_fixedDt = std::chrono::microseconds(1000000 / k_tps);
		static constexpr sf::Time k_maxAccumulated{ std::chrono::milliseconds(200) };
		
		ContextObject m_context;
		sf::RenderWindow m_window;
		SceneManager<SceneTagEnum, ContextObject> m_scenes;
		Input m_input;
		sf::Clock m_clock;
		sf::Time m_accumulated;
	};
}