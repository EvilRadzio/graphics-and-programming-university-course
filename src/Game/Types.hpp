#pragma once

#include "pXL/pXL.hpp"

#include "Context.hpp"
#include "SceneId.hpp"

struct Internal
{
	using Context = Context;
	using SceneId = SceneId;
};

using Scene = px::Scene<Internal>;