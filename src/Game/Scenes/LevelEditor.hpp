#pragma once

#include "Game/Types.hpp"

namespace Scenes
{
	class LevelEditor : public Scene
	{
	public:
			
		LevelEditor(ApiScene api, Context& ctx);

		void update(px::ApiUpdate& api) override;
		void draw(px::ApiDraw& api) const override;
	private:

		px::Grid<Tile> LE_map;
		int currentTile = 0;
		int currentMap = 1;
		std::vector<std::string> maps = {
			"./resources/maps/1.txt",
			"./resources/maps/2.txt",
			"./resources/maps/3.txt",
		};
		std::vector<std::string> TileName;
		std::vector<const char*> TileNameC;
		std::string mapName = "./resources/maps/" + std::to_string(currentMap) + ".txt";

	};
}