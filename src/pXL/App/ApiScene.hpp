#pragma once

namespace px
{
	class InputRaw;
	class Assets;

	struct ApiScene
	{
		const InputRaw& input;
		const Assets& assets;
	};
}