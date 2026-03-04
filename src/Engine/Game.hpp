#pragma once

#include <stdint.h>
#include <algorithm>

#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>

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
			m_window(sf::VideoMode(sf::Vector2u{ 720,720 }), "Game", sf::Style::Close),
			m_tileTextures(m_textures)
		{
			ImGui::SFML::Init(m_window);
			ImGui::GetIO().FontGlobalScale = 2.0f;
		};

		virtual ~Game()
		{
			ImGui::SFML::Shutdown();
		}

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

					ImGui::SFML::ProcessEvent(m_window, *event);

					if (event->is<sf::Event::Closed>())
					{
						m_window.close();
					}
				}

				sf::Time realDt = m_clock.restart();

				Apis::UpdateGui updateGuiApi
				{
					m_window,
					m_tiles
				};

				ImGui::SFML::Update(m_window, realDt);

				m_scenes.updateGui(m_context, updateGuiApi);

				m_accumulated += realDt;
				m_accumulated = std::min(m_accumulated, k_maxAccumulated);

				while (m_accumulated >= k_fixedDt)
				{
					m_accumulated -= k_fixedDt;

					Apis::Update updateApi{
						m_input,
						k_fixedDt
					};

					m_scenes.update(m_context, updateApi);

					m_input.newFrame();
				}

				m_window.clear(sf::Color::Black);

				Apis::Draw drawApi{
					m_window,
					m_textures,
					m_tiles,
					m_tileTextures,
					m_input,
					m_font
				};

				m_scenes.draw(m_context, drawApi);

				ImGui::SFML::Render(m_window);

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