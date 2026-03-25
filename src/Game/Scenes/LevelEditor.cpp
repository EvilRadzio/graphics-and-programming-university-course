#include "LevelEditor.hpp"

void Scenes::LevelEditor::update(px::ApiUpdate& api)
{
	if (ImGui::Begin("Editor Menu"))
	{
		if (ImGui::Button("Back"))
		{

			popScene();
		}
	}
	ImGui::End();
}

void Scenes::LevelEditor::draw(px::ApiDraw& api) const
{

}