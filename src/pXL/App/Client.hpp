#pragma once

#include <stdint.h>
#include <algorithm>

#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>

#include "SceneStack.hpp"
#include "InputRaw.hpp"

namespace px
{
	template <Internal I>
	class Client
	{
	public:

		Client() :
			window(sf::VideoMode(sf::Vector2u{ 720,720 }), "Game", sf::Style::Close)
		{
			window.setKeyRepeatEnabled(false);
			window.setFramerateLimit(60);
			ImGui::SFML::Init(window);
			ImGui::GetIO().FontGlobalScale = 2.0f;
		};

		Client(const Client&) = delete;
		Client(Client&&) = delete;
		Client& operator=(const Client&) = delete;
		Client& operator=(Client&&) = delete;

		virtual ~Client()
		{
			ImGui::SFML::Shutdown();
		}

		virtual void preUpdate(){}
		virtual void postUpdate(){}
		virtual void preDraw(){}
		virtual void postDraw(){}

		void run()
		{
			sf::Clock clock;

			clock.restart();

			while (window.isOpen())
			{
				input.newTick();

				while (const auto event = window.pollEvent())
				{
					input.readEvent(*event);

					ImGui::SFML::ProcessEvent(window, *event);

					if (event->is<sf::Event::Closed>())
					{
						window.close();
					}
				}

				sf::Time realDt = clock.restart();

				ImGui::SFML::Update(window, realDt);

				ApiUpdate updateApi{
					window,
					k_fixedDt
				};

				preUpdate();

				scenes.update(updateApi, sceneComms);

				postUpdate();

				preDraw();

				window.clear(sf::Color::Black);

				ApiDraw drawApi{
					window,
					assets
				};

				scenes.draw(drawApi);

				ImGui::SFML::Render(window);

				window.display();

				postDraw();
			}
		}

	protected:

		typename I::Context ctx;

		Assets assets;
		sf::RenderWindow window;
		SceneStack<I> scenes;
		InputRaw input;
		SceneComms<I> sceneComms;

		ApiScene<I> apiScene{
			sceneComms,
			input,
			assets
		};

	private:

		static constexpr uint32_t k_tps{ 60 };
		static constexpr sf::Time k_fixedDt = sf::microseconds(1000000 / k_tps);
	};
}