#pragma once

#include "Game/Types.hpp"

namespace Scenes
{
	class LevelEditor : public px::Scene
	{
	public:
			
		LevelEditor(px::ApiScene api, Context& ctx);

		void update(px::ApiUpdate& api) override;
		void draw(px::ApiDraw& api) const override;
		void resizeMap();
	private:
		Context& ctx;
		sf::Vector2i lastMousePos;
		sf::Vector2i viewPosition{0,0};
		int windowSize = 720;
		int mapWidth = 25;
		int mapHeight = 25;
		px::Grid<Tile> LE_map;
		int currentTile = 0;
		int currentMap = 1;
		std::vector<std::string> maps = {
			"./resources/maps/1.txt",
			"./resources/maps/2.txt",
			"./resources/maps/3.txt",
			"./resources/maps/4.txt",
			"./resources/maps/5.txt",
		};
		std::vector<std::string> TileName;
		std::vector<const char*> TileNameC;
		std::string mapName = "./resources/maps/" + std::to_string(currentMap) + ".txt";

	};
}