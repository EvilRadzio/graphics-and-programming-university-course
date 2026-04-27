#pragma once

#include <stdint.h>
#include <algorithm>
#include <filesystem>

#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
#include <spdlog/spdlog.h>

#include "SceneStack.hpp"
#include "Transition.hpp"
#include "Input.hpp"

namespace px
{
	class Client
	{
	public:

		void run();

	protected:

		Client();
		virtual ~Client();

		Client(const Client&) = delete;
		Client(Client&&) = delete;
		Client& operator=(const Client&) = delete;
		Client& operator=(Client&&) = delete;

		virtual void interceptEvent(const sf::Event& event) {}

		virtual void preEvent() {}
		virtual void postEventPreUpdate() {}
		virtual void postUpdatePreDraw() {}
		virtual void postDraw() {}

		void recursiveLoad(const std::string& directoryPath,
			std::function<void(const std::filesystem::path& path,
				const std::string& name)>&& call);

		struct ScaleSettings
		{
			sf::Vector2f minimumUnits{ 20.0f, 10.25f };
			uint32_t pixelsPerUnit{ 16 };
		};

		Assets assets;
		sf::RenderWindow window;
		SceneStack scenes;
		Input frameInput;
		Input tickInput;
		Mapping mapping{ frameInput };
		Transition transition;

		ScaleSettings scaleSettings;

		float unit{};
		float baseMultiplier{};

		EngineApi engApi{
			scenes,
			assets,
			mapping,
			unit
		};

		SceneInitCtx apiScene{
			scenes,
			transition,
			engApi
		};

		bool showFps{};

	private:

		static constexpr uint32_t k_tps{ 60 };
		static constexpr sf::Time k_fixedDt = sf::microseconds(1000000 / k_tps);
	};

	inline Client::Client() :
		window(sf::VideoMode(sf::Vector2u{ 1280,720 }), "Game", sf::Style::Default)
	{
		window.setKeyRepeatEnabled(false);
		ImGui::SFML::Init(window);
		ImGui::GetIO().FontGlobalScale = 2.0f;

		scenes.setOnChangeCallback([&]() {
			frameInput.newUpdate();
			tickInput.newUpdate();
		});

		auto [minUnits, pixelsPerUnit] = scaleSettings;
		auto minimumWindowSize = static_cast<sf::Vector2u>(minUnits * static_cast<float>(pixelsPerUnit));
		window.setMinimumSize(minimumWindowSize);
		auto windowSize = static_cast<sf::Vector2f>(window.getSize());
		sf::Vector2f ratios{
			windowSize.x / minimumWindowSize.x,
			windowSize.y / minimumWindowSize.y
		};
		baseMultiplier = static_cast<uint32_t>(std::min(ratios.x, ratios.y));
		unit = pixelsPerUnit * baseMultiplier;
	}

	inline Client::~Client()
	{
		ImGui::SFML::Shutdown();
	}

	inline void Client::run()
	{
		sf::Clock clock;
		sf::Time acumulator;

		while (window.isOpen())
		{
			scenes.flush();

			preEvent();

			frameInput.newUpdate();

			while (const auto event = window.pollEvent())
			{
				frameInput.readEvent(*event);
				tickInput.readEvent(*event);

				ImGui::SFML::ProcessEvent(window, *event);

				interceptEvent(*event);

				if (event->is<sf::Event::Closed>())
				{
					window.close();
				}
			}

			auto [minUnits, pixelsPerUnit] = scaleSettings;
			auto minimumWindowSize = static_cast<sf::Vector2u>(minUnits * static_cast<float>(pixelsPerUnit));
			window.setMinimumSize(minimumWindowSize);
			auto windowSize = static_cast<sf::Vector2f>(window.getSize());
			sf::Vector2f ratios{
				windowSize.x / minimumWindowSize.x,
				windowSize.y / minimumWindowSize.y
			};
			baseMultiplier = static_cast<uint32_t>(std::min(ratios.x, ratios.y));
			unit = pixelsPerUnit * baseMultiplier;

			postEventPreUpdate();

			sf::Time realDt = clock.restart();

			ImGui::SFML::Update(window, realDt);

			mapping.setUnderlyingInput(frameInput);

			transition.update(realDt.asSeconds());

			UpdateCtx updateCtx{
				window,
				realDt,
				transition
			};

			scenes.update(updateCtx);

			acumulator = std::min(acumulator + realDt, k_fixedDt * 4.001f);

			mapping.setUnderlyingInput(tickInput);

			UpdateCtx fixedUpdateCtx{
				window,
				k_fixedDt,
				transition
			};

			while (acumulator >= k_fixedDt)
			{
				scenes.fixedUpdate(fixedUpdateCtx);
				tickInput.newUpdate();
				acumulator -= k_fixedDt;
			}

			postUpdatePreDraw();

			float alpha = acumulator / k_fixedDt;

			DrawCtx drawCtx{
				window,
				assets,
				alpha,
				unit,
				baseMultiplier
			};

			window.clear(sf::Color::Black);

			scenes.draw(drawCtx);

			ImGui::SFML::Render(window);

			window.draw(transition);

			if (showFps)
			{
				sf::Text fpsDisplay(assets.font, std::to_string(1.f / realDt.asSeconds()));
				window.draw(fpsDisplay);
			}

			postDraw();

			window.display();
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