#pragma once

#include "Game/Schema/Types.hpp"

namespace Game::Scenes
{
	class MainMenu : public Schema::Scene
	{
	public:

		void updateGui(Schema::Context& context, Engine::Apis::UpdateGui& api) override
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();

			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);

			if (ImGui::Begin("##Menu", nullptr,
				ImGuiWindowFlags_NoDecoration |
				ImGuiWindowFlags_NoMove |
				ImGuiWindowFlags_NoResize |
				ImGuiWindowFlags_NoSavedSettings |
				ImGuiWindowFlags_NoBackground))
			{
				if (ImGui::Button("Play TicTacToe"))
				{
					pushScene(Schema::SceneId::TicTacToe);
				}
				if (ImGui::Button("Tilemap Editor"))
				{
					pushScene(Schema::SceneId::LevelEditor);
				}
				if (ImGui::Button("Exit"))
				{
					api.window.close();
				}
			}
			ImGui::End();
		}

		void update(Schema::Context& context, Engine::Apis::Update& api) override
		{
			auto mousePos = api.input.getMousePosition();

			sf::Rect<float> rect(sf::Vector2f(0,0), sf::Vector2f(10,10));

			if (api.input.isPressed(sf::Mouse::Button::Left) &&
				rect.contains((sf::Vector2f)mousePos))
			{

			}
		}

		void draw(const Schema::Context& context, Engine::Apis::Draw& api) const override
		{
			api.window.clear(sf::Color(0x222222ff));

			sf::RectangleShape mikuShape(static_cast<sf::Vector2f>(api.window.getSize()));
			mikuShape.setTexture(&api.textures.getTexture("player"));

			api.window.draw(mikuShape);
		}
	};
}