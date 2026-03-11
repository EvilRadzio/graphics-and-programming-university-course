#pragma once

#include "Game/Types.hpp"

namespace Scenes
{
	class LevelEditor : public Scene
	{
	private:
		px::Map LE_map;
		int currentTile = 0;
	public:
			
		LevelEditor(px::ApiScene api) : Scene(api), LE_map(sceneApi.tiles.emptyHandle(), 25, 25) {}

		void updateGui(Context& context, px::ApiUpdateGui& api) override;
		void update(Context& context, px::ApiUpdate& api) override;
		void draw(const Context& context, px::ApiDraw& api) const override;
	};
}