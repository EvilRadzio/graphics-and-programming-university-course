#include "LevelEditor.hpp"

void Scenes::LevelEditor::updateGui(Context& context, px::ApiUpdateGui& api)
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

void Scenes::LevelEditor::update(Context& context, px::ApiUpdate& api)
{

}

void Scenes::LevelEditor::draw(const Context& context, px::ApiDraw& api) const
{

}