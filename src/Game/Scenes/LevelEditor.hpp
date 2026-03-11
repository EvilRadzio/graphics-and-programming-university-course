#pragma once

#include "Game/Types.hpp"

namespace Scenes
{
	class LevelEditor : public Scene
	{
	public:

		LevelEditor(px::ApiScene api) : Scene(api) {}

		void updateGui(Context& context, px::ApiUpdateGui& api) override;
		void update(Context& context, px::ApiUpdate& api) override;
		void draw(const Context& context, px::ApiDraw& api) const override;
	};
}