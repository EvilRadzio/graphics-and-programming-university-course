#pragma once

#include "Game/Types.hpp"

namespace Scenes
{
	class LevelEditor : public Scene
	{
	public:
			
		LevelEditor(px::ApiScene api, Context& ctx) : Scene(api, ctx) {}

		void update(px::ApiUpdate& api) override;
		void draw(px::ApiDraw& api) const override;
	};
}