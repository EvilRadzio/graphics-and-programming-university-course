#include "LevelEditor.hpp"
#include <iostream>
#include <fstream>
#include <filesystem>

Scenes::LevelEditor::LevelEditor(ApiScene api, Context& ctx) :
	Scene(api, ctx),
	LE_map(sf::Vector2u(25, 25), ctx.tiles["empty"]) 
{
	for (const auto& [tilename, _ ] : ctx.tiles) {
		TileName.push_back(tilename);
	}
	//for (int i = 0; i < TileName.size(); i++) { TileNameC[i] = TileName.at(i); }
	std::fstream loadedMap(mapName);
	if (!loadedMap) {
		std::cout << "bad file" << std::endl;
	}
	for (unsigned int y = 0; y < 25; y++) {
		for (unsigned int x = 0; x < 25; x++)
		{
			std::string loadedId;
			loadedMap >> loadedId;
			if (ctx.tiles.contains(loadedId)) {
				LE_map.at({ x,y }) = ctx.tiles.at(loadedId);
			}
			else {
				std::cout << "Unknown Tile" << std::endl;
				LE_map.at({ x,y }) = ctx.tiles.at("empty");
			}
		}
	}
}

void Scenes::LevelEditor::update(px::ApiUpdate& api)
{
	if (ImGui::Begin("Editor Menu"))
	{
		if (ImGui::Button("Back"))
		{
			scene.comms.pop({});
		}

		//ImGui::DragInt("##d", &currentTile, 0.5f, 0, 1);

		//----
		static bool item_highlight = false;
		int item_highlighted_idx = -1;
		if (ImGui::BeginListBox("Choose tile"))
		{
			for (int n = 0; n < TileName.size(); n++)
			{
				const bool is_selected = (currentTile == n);
				if (ImGui::Selectable(TileName[n].c_str(), is_selected))
					currentTile = n;
				if (item_highlight && ImGui::IsItemHovered())
					item_highlighted_idx = n;

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndListBox();
			
			if (ImGui::BeginListBox("Choose map"))
			{
				for (int n = 0; n < maps.size(); n++)
				{
					const bool is_selected = (currentMap == n);
					if (ImGui::Selectable(maps[n].c_str(), is_selected))
						currentMap = n;
					if (item_highlight && ImGui::IsItemHovered())
						item_highlighted_idx = n;

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndListBox();
			}
			//ImGui::DragInt("##mCurrentMap", &currentMap, 1.f, 0, 2);


			if (ImGui::Button("Save")) {

				//std::filesystem::path path(mapName);
				//std::cout << std::filesystem::absolute(path) << std::endl;	//Check File Path
				std::ofstream Save(mapName);
				
				
				for (unsigned int x = 0; x < 25; x++) {
					for (unsigned int y = 0; y < 25; y++) {
						Save << LE_map.at({ x , y }).tileName << " ";
					}
					Save << std::endl;
				}
			}

			if (ImGui::Button("Load")) {

				//std::filesystem::path path(mapName);
				//std::cout << std::filesystem::absolute(path) << std::endl;	//Check File Path
				std::ifstream LoadedMap(mapName);
				if (LoadedMap.fail()) {
					std::cout << "No such map" << std::endl;
				}
				for (unsigned int y = 0; y < 25; y++) {
					for (unsigned int x = 0; x < 25; x++)
					{
						std::string loadedId;
						LoadedMap >> loadedId;
						if (ctx.tiles.contains(loadedId)) {
							LE_map.at({ x,y }) = ctx.tiles.at(loadedId);
						}
						else {
							std::cout << "Unknown Tile" << std::endl;
							LE_map.at({ x,y }) = ctx.tiles.at("empty");
						}
					}
				}
			}
		}
		ImGui::End();
	}

	sf::Rect<int> rect{
		sf::Vector2i(0,0), sf::Vector2i(28,28)
	};

	sf::Vector2i mousePosition = scene.input.getMousePosition();
	for (unsigned int y = 0; y < 25; y++) {
		for (unsigned int x = 0; x < 25; x++) {
			rect.position = sf::Vector2i(x * 28, y * 28);
			if (scene.input.isPressed(sf::Mouse::Button::Left) && rect.contains(mousePosition)) {
				sf::Vector2u MP = sf::Vector2u(mousePosition / (720 / 25));
				LE_map.at(MP) = ctx.tiles.at(TileName[currentTile]);
				//LE_map.set(MP, sceneApi.tiles.handle("solid_block")); //change it to currentTile later.

			}
		}
	}
}

void Scenes::LevelEditor::draw(px::ApiDraw& api) const
{
	uint32_t tileSide = 720 / LE_map.size().x;

	for (size_t y = 0; y < LE_map.size().y; ++y) for (size_t x = 0; x < LE_map.size().x; ++x)
	{
		sf::Vector2u position(x, y);
		if (LE_map.at(position).sprite != "")
		{
			auto sprite(api.assets.tileSprites.get(LE_map.at(position).sprite).get(0, api.assets.textures));

			sprite.setPosition(sf::Vector2f{
				static_cast<float>(x * tileSide),
				static_cast<float>(y * tileSide)
			});

			auto bounds = sprite.getLocalBounds();
			sprite.setScale(sf::Vector2f{
				static_cast<float>(tileSide) / bounds.size.x,
				static_cast<float>(tileSide) / bounds.size.y
			});

			api.window.draw(sprite);
		}
	}
}