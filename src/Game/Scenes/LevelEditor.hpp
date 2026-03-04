#pragma once

#include "Game/Schema/Types.hpp"
#include "Engine/Map.hpp"

namespace Game::Scenes
{
	class LevelEditor : public Schema::Scene
	{
	public:

		LevelEditor(Engine::Map&& map) : m_map(std::move(map)) {}

		void updateGui(Schema::Context& context, Engine::Apis::UpdateGui& api) override;
		void update(Schema::Context& context, Engine::Apis::Update& api) override;
		void draw(const Schema::Context& context, Engine::Apis::Draw& api) const override;

	private:

		Engine::Map m_map;
	};
}