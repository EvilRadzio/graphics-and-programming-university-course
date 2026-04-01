#pragma once

#include "SceneComms.hpp"

namespace px
{
	class InputRaw;
	class Assets;

	template <Internal I>
	class SceneCommands;

	template <Internal I>
	struct ApiScene
	{
		SceneCommands<I>& comms;
		const InputRaw& input;
		const Assets& assets;
	};
}