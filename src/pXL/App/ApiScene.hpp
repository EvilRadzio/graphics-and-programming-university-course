#pragma once

#include "SceneComms.hpp"

namespace px
{
	class InputRaw;
	class Assets;

	template <Internal I>
	struct ApiScene
	{
		SceneComms<I>& comms;
		const InputRaw& input;
		const Assets& assets;
	};
}