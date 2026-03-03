#pragma once

#include "Game/Schema/Types.hpp"

namespace Game::Scenes
{
	class LevelEditor : public Schema::Scene
	{
	public:

		void updateImgui(Schema::Context& context, Engine::UpdateApi& api) override;
		void update(Schema::Context& context, Engine::UpdateApi& api) override;
		void draw(const Schema::Context& context, Engine::DrawApi& api) const override;
	};
}