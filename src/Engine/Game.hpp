#pragma once

#include <stdint.h>
#include <algorithm>

#include <SFML/Graphics.hpp>

#include "SceneManager.hpp"
#include "TextureManager.hpp"
#include "TileManager.hpp"
#include "TileTextureManager.hpp"

namespace Engine
{
	template <Internal I>
	class Game
	{
	public:

		Game() :
			m_window(sf::VideoMode(sf::Vector2u{ 720,720 }), "Game"),
			m_tileTextures(m_textures)
		{
			
		};

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

					UpdateApi updateApi{
						m_input
					};

					m_scenes.update(m_context, updateApi);

					m_input.newFrame();
				}

				m_window.clear(sf::Color::Black);

				DrawApi drawApi{
					m_window,
					m_textures,
					m_tiles,
					m_tileTextures,
					m_input,
					m_font
				};

				m_scenes.draw(m_context, drawApi);

				m_window.display();
			}
		}

	protected:

		typename I::Context m_context;
		TextureManager m_textures;
		sf::RenderWindow m_window;
		SceneManager<I> m_scenes;
		TileTextureManager m_tileTextures;
		TileManager m_tiles;
		Input m_input;
		sf::Font m_font;

	private:

		static constexpr uint32_t k_tps{ 60 };
		static constexpr sf::Time k_fixedDt = std::chrono::microseconds(1000000 / k_tps);
		static constexpr sf::Time k_maxAccumulated{ std::chrono::milliseconds(200) };
		
		sf::Clock m_clock;
		sf::Time m_accumulated;
	};
}