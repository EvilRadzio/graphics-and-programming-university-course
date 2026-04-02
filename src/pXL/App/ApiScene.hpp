#pragma once

#include "Internal.hpp"

namespace px
{
	class InputRaw;
	class Assets;

	template <Internal I>
	class SceneCommands;

	class SceneConfig;

	template <Internal I>
	struct ApiScene
	{
		SceneCommands<I>& comms;
		const InputRaw& input;
		const Assets& assets;
		SceneConfig& properties;
	};
}