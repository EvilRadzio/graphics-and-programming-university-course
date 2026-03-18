#pragma once

#include "Game/Types.hpp"

namespace Scenes
{
	class LevelEditor : public Scene
	{
	private:
		px::Map LE_map;
		int currentTile = 0;
		int currentMap = 1;
		std::vector<px::TileHandle> tileId;
		std::string mapName;


	public:
			
		LevelEditor(px::ApiScene api);
		
		void updateGui(Context& context, px::ApiUpdateGui& api) override;
		void update(Context& context, px::ApiUpdate& api) override;
		void draw(const Context& context, px::ApiDraw& api) const override;
	};
}