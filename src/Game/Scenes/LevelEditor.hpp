#pragma once

#include "Game/Types.hpp"

namespace Scenes
{
	class LevelEditor : public Scene
	{
	private:
		px::IdMap LE_map;
		int currentTile = 0;
	public:
			
		LevelEditor(px::ApiScene api, Context& ctx) : Scene(api, ctx), LE_map(sf::Vector2u(25, 25)) {}

		void updateGui(px::ApiUpdateGui& api) override;
		void update(px::ApiUpdate& api) override;
		void draw(px::ApiDraw& api) const override;
	};
}