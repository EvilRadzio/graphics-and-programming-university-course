#pragma once

#include <stdint.h>
#include <algorithm>
#include <filesystem>

#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>

#include "SceneStack.hpp"
#include "InputRaw.hpp"
#include "Transition.hpp"

namespace px
{
	class Client
	{
	public:

		Client();
		virtual ~Client();

		Client(const Client&) = delete;
		Client(Client&&) = delete;
		Client& operator=(const Client&) = delete;
		Client& operator=(Client&&) = delete;

		void run();

	protected:

		virtual void interceptEvent(const sf::Event& event) {}

		virtual void preEvent() {}
		virtual void postEventPreUpdate() {}
		virtual void postUpdatePreDraw() {}
		virtual void postDraw() {}

		void recursiveLoad(const std::string& directoryPath,
			std::function<void(const std::filesystem::path& path,
				const std::string& name)>&& call);

		Assets assets;
		sf::RenderWindow window;
		SceneStack scenes;
		InputRaw input;
		Transition transition;

		ApiScene apiScene{
			scenes,
			input,
			assets,
			scenes,
			transition
		};

	private:

		static constexpr uint32_t k_tps{ 60 };
		static constexpr sf::Time k_fixedDt = sf::microseconds(1000000 / k_tps);
	};

	inline Client::Client() :
		window(sf::VideoMode(sf::Vector2u{ 720,720 }), "Game", sf::Style::Close)
	{
		window.setKeyRepeatEnabled(false);
		window.setFramerateLimit(60);
		ImGui::SFML::Init(window);
		ImGui::GetIO().FontGlobalScale = 2.0f;
	}

	inline Client::~Client()
	{
		ImGui::SFML::Shutdown();
	}

	inline void Client::run()
	{
		sf::Clock clock;

		while (window.isOpen())
		{
			scenes.flush();

			preEvent();

			input.newTick();

			while (const auto event = window.pollEvent())
			{
				input.readEvent(*event);

				ImGui::SFML::ProcessEvent(window, *event);

				interceptEvent(*event);

				if (event->is<sf::Event::Closed>())
				{
					window.close();
				}
			}

			postEventPreUpdate();

			sf::Time realDt = clock.restart();

			ImGui::SFML::Update(window, realDt);

			transition.update(k_fixedDt.asSeconds());

			ApiUpdate updateApi{
				window,
				k_fixedDt,
				transition
			};

			scenes.update(updateApi);

			postUpdatePreDraw();

			window.clear(sf::Color::Black);

			ApiDraw drawApi{
				window,
				assets
			};

			scenes.draw(drawApi);

			ImGui::SFML::Render(window);

			window.draw(transition);

			window.display();

			postDraw();
		}
	}

	inline void Client::recursiveLoad(const std::string& directoryPath, std::function<void(const std::filesystem::path& path, const std::string& name)>&& call)
	{
		if (!std::filesystem::exists(directoryPath))
		{
			return;
		}

		for (const auto& entry : std::filesystem::recursive_directory_iterator(directoryPath))
		{
			if (!entry.is_regular_file())
			{
				continue;
			}

			std::string name = std::filesystem::relative(entry.path(), directoryPath).replace_extension().generic_string();

			call(entry.path(), name);
		}
	}
}