#pragma once

#include <stdint.h>
#include <algorithm>

#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>

#include "Engine/Scenes/Scenes.hpp"
#include "Engine/Resources/Textures/Textures.hpp"
#include "Engine/World/World.hpp"
#include "Input/Input.hpp"

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
				m_input.newTick();

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
				};

				ImGui::SFML::Update(m_window, realDt);

				m_scenes.updateGui(m_context, updateGuiApi);

				m_accumulated += realDt;
				m_accumulated = std::min(m_accumulated, k_maxAccumulated);

				while (m_accumulated >= k_fixedDt)
				{
					m_accumulated -= k_fixedDt;

					Apis::Update updateApi{
						m_window,
						k_fixedDt
					};

					m_scenes.update(m_context, updateApi);

					m_input.newTick();
				}

				m_window.clear(sf::Color::Black);

				Apis::Draw drawApi{
					m_window,
					m_textures,
					m_tiles,
					m_tileTextures,
					m_font
				};

				m_scenes.draw(m_context, drawApi);

				ImGui::SFML::Render(m_window);

				m_window.display();
			}
		}

	protected:

		Apis::Scene buildSceneApi()
		{
			return Apis::Scene{
				m_input,
				m_tiles
			};
		}

		typename I::Context m_context;
		Resources::Textures::Manager m_textures;
		sf::RenderWindow m_window;
		Scenes::Manager<I> m_scenes;
		World::Tiles::TextureManager m_tileTextures;
		World::Tiles::Manager m_tiles;
		Input::Raw m_input;
		sf::Font m_font;

	private:

		static constexpr uint32_t k_tps{ 60 };
		static constexpr sf::Time k_fixedDt = std::chrono::microseconds(1000000 / k_tps);
		static constexpr sf::Time k_maxAccumulated{ std::chrono::milliseconds(200) };
		
		sf::Clock m_clock;
		sf::Time m_accumulated;
	};
}