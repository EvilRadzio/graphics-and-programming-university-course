#include "LevelEditor.hpp"

void Game::Scenes::LevelEditor::update(Schema::Context& context, Engine::UpdateApi& api)
{
	if (api.input.isPressed(sf::Keyboard::Scan::Escape))
	{
		popScene();
	}
}

void Game::Scenes::LevelEditor::draw(const Schema::Context& context, Engine::DrawApi& api) const
{
	api.window.clear(sf::Color::Green);

	sf::RectangleShape textureRec(sf::Vector2f(500, 500));
	textureRec.move(sf::Vector2f(100, 100));
	textureRec.setTexture(&api.textures.getTexture("solid_block"));

	api.window.draw(textureRec);
}