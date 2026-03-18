#include "LevelEditor.hpp"
#include <fstream>
#include <filesystem>

//dodaj od kontruktora wczytywanie mapy przez konstruktor
Scenes::LevelEditor::LevelEditor(px::ApiScene api) : Scene(api), LE_map(sceneApi.tiles.emptyHandle(), 25, 25) 
{
	
	for (const auto& [handle, tile] : api.tiles) {
		tileId.push_back(handle);
	}
	mapName = "./resources/maps/" + std::to_string(currentMap) + ".txt";
	std::fstream loadedMap(mapName);
	if (!loadedMap) {
		std::cout << "bad file" << std::endl;
	}
	for (int y = 0; y < 25; y++) {
		for (int x = 0; x < 25; x++) 
		{
			unsigned int loadedId;
			loadedMap >> loadedId;
			LE_map.set(sf::Vector2u(x, y), tileId[loadedId]);
		}
	}
	
}


void Scenes::LevelEditor::updateGui(Context& context, px::ApiUpdateGui& api)
{
	if (ImGui::Begin("Editor Menu"))
	{
		ImGui::DragInt("##d", &currentTile, 0.5f, 0, 1);
		if (ImGui::Button("Back"))
		{
			
			popScene();
		}
		//map change via name
		//char mapName[128] = "Nazwa pliku";
		//ImGui::InputText("input text", mapName, sizeof(mapName) / sizeof(char));
		ImGui::DragInt("##m", &currentMap, 0.5f, 0, 1);
		if (ImGui::Button("Save")) {
			
			std::filesystem::path path(mapName);
			std::cout << std::filesystem::absolute(path) << std::endl;
			std::ofstream Save(path);
			for (int x=0; x < 25; x++) {
				for (int y=0; y < 25; y++) {
					Save << LE_map.at(sf::Vector2u(x,y)).id;
					Save << " ";
				}
				Save << std::endl;
			}
		}
		//if (ImGui::Button("Load")) { }
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
		}
	}
}
//vector tilehandle
void Scenes::LevelEditor::draw(const Context& context, px::ApiDraw& api) const
{
	px::DrawMap map(LE_map, api.tileTextures, api.textures);
	
	sf::RectangleShape tileRect(static_cast<sf::Vector2f>(api.window.getSize()) / 25.0f);
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