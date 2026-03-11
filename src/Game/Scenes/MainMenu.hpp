#pragma once

#include "Game/Types.hpp"

namespace Scenes
{
	class MainMenu : public Scene
	{
	public:

		MainMenu(px::ApiScene api) : Scene(api) {}

		void updateGui(Context& context, px::ApiUpdateGui& api) override
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
					pushScene(SceneId::TicTacToe);
				}
				if (ImGui::Button("Platforming!!!"))
				{
					pushScene(SceneId::Platforming);
				}
				if (ImGui::Button("Tilemap Editor"))
				{
					pushScene(SceneId::LevelEditor);
				}
				if (ImGui::Button("Exit"))
				{
					//api.window.close();
				}
			}
			ImGui::End();
		}

		void update(Context& context, px::ApiUpdate& api) override
		{

		}

		void draw(const Context& context, px::ApiDraw& api) const override
		{
			api.window.clear(sf::Color(0x222222ff));

			sf::RectangleShape mikuShape(static_cast<sf::Vector2f>(api.window.getSize()));
			mikuShape.setTexture(&api.textures.texture("player"));

			api.window.draw(mikuShape);
		}
	};
}