#pragma once

#include "Game/Schema/Types.hpp"

namespace Game::Scenes
{
	class LevelEditor : public Schema::Scene
	{
	public:

		void updateGui(Schema::Context& context, Engine::Apis::UpdateGui& api) override;
		void update(Schema::Context& context, Engine::Apis::Update& api) override;
		void draw(const Schema::Context& context, Engine::Apis::Draw& api) const override;
	};
}