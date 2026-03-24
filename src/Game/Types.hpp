#pragma once

#include "pXL/pXL.hpp"

#include "Context.hpp"
#include "SceneId.hpp"
#include "Tile.hpp"

struct Internal
{
	using Context = Context;
	using SceneId = SceneId;
	using ScenePayload = int;
};

using Scene = px::Scene<Internal>;