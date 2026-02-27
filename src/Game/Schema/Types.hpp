#pragma once

#include "Engine/Game.hpp"

#include "Context.hpp"
#include "SceneId.hpp"

namespace Game::Schema
{
	struct Internal
	{
		using Context = Schema::Context;
		using SceneId = Schema::SceneId;
	};

	using Scene = Engine::Scene<Internal>;
}