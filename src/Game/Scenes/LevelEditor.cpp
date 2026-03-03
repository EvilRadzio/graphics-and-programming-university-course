#include "LevelEditor.hpp"

void Game::Scenes::LevelEditor::updateGui(Schema::Context& context, Engine::Apis::UpdateGui& api)
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

void Game::Scenes::LevelEditor::update(Schema::Context& context, Engine::Apis::Update& api)
{
	if (api.input.isPressed(sf::Keyboard::Scan::Escape))
	{
		popScene();
	}
}

void Game::Scenes::LevelEditor::draw(const Schema::Context& context, Engine::Apis::Draw& api) const
{

}