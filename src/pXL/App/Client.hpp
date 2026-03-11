#pragma once

#include <stdint.h>
#include <algorithm>

#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>

#include "SceneManager.hpp"
#include "pXL/Resources/TextureManager.hpp"
#include "InputRaw.hpp"

namespace px
{
	template <Internal I>
	class Client
	{
	public:

		Client() :
			m_window(sf::VideoMode(sf::Vector2u{ 720,720 }), "Game", sf::Style::Close),
			m_tileTextures(m_textures)
		{
			m_window.setKeyRepeatEnabled(false);
			ImGui::SFML::Init(m_window);
			ImGui::GetIO().FontGlobalScale = 2.0f;
		};

		virtual ~Client()
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

				ApiUpdateGui updateGuiApi
				{
				};

				ImGui::SFML::Update(m_window, realDt);

				m_scenes.updateGui(m_context, updateGuiApi);

				ApiUpdate updateApi{
					m_window,
					k_fixedDt
				};

				m_scenes.update(m_context, updateApi);

				m_window.clear(sf::Color::Black);

				ApiDraw drawApi{
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

		ApiScene buildSceneApi()
		{
			return ApiScene{
				m_input,
				m_tiles
			};
		}

		typename I::Context m_context;
		TextureManager m_textures;
		sf::RenderWindow m_window;
		SceneManager<I> m_scenes;
		TileTextureManager m_tileTextures;
		TileManager m_tiles;
		InputRaw m_input;
		sf::Font m_font;

	private:

		static constexpr uint32_t k_tps{ 60 };
		static constexpr sf::Time k_fixedDt = std::chrono::microseconds(1000000 / k_tps);
		static constexpr sf::Time k_maxAccumulated{ std::chrono::milliseconds(200) };
		
		sf::Clock m_clock;
		sf::Time m_accumulated;
	};
}