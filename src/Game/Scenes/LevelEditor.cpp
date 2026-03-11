#include "LevelEditor.hpp"



void Scenes::LevelEditor::updateGui(Context& context, px::ApiUpdateGui& api)
{
	if (ImGui::Begin("Editor Menu"))
	{
		ImGui::DragInt("##d", &currentTile, 0.5f, 0, 1);
		if (ImGui::Button("Back"))
		{
			
			popScene();
		}
	}
	ImGui::End();
}

void Scenes::LevelEditor::update(Context& context, px::ApiUpdate& api)
{
	sf::Rect<int> rect{
		sf::Vector2i(0,0), sf::Vector2i(28,28)
	};
	sf::Vector2i mousePosition = sceneApi.input.getMousePosition();
	for (int y = 0; y < 25; y++) {
		for (int x = 0; x < 25; x++) {
			rect.position = sf::Vector2i(x * 28, y * 28);
			if ( sceneApi.input.isPressed(sf::Mouse::Button::Left ) && rect.contains(mousePosition)) {
				sf::Vector2u MP = sf::Vector2u(mousePosition / (720 / 25)) ;
				LE_map.set(MP, sceneApi.tiles.handle("solid_block")); //change it to currentTile later.
			}
			//if (sceneApi.input.isPressed(sf::Mouse::Button::Right) && rect.contains(mousePosition)) {
			//	sf::Vector2u MP = sf::Vector2u(mousePosition / (720 / 25));
			//	LE_map.set(MP, sceneApi.tiles.handle("")); 
				
			//}
		}
	}
}
//vector tilehandle
void Scenes::LevelEditor::draw(const Context& context, px::ApiDraw& api) const
{
	//px::DrawMap map(LE_map, api.tileTextures, sceneApi.tiles);

	sf::RectangleShape tileRect(static_cast<sf::Vector2f>(api.window.getSize()) / 10.0f);
	uint32_t tileSide = 720 / LE_map.width();

	for (size_t y = 0; y < LE_map.height(); ++y) for (size_t x = 0; x < LE_map.width(); ++x)
	{
		sf::Vector2u position(x, y);
		px::TileHandle handle = LE_map.at(position);
		if (api.tileTextures.hasTexture(handle))
		{
			tileRect.setPosition(static_cast<sf::Vector2f>(position * tileSide));
			tileRect.setTexture(&api.textures.texture(api.tileTextures.handle(handle)));

			api.window.draw(tileRect);
		}
	}
}