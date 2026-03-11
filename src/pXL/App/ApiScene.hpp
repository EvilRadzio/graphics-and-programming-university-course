#pragma once

namespace px
{
	class InputRaw;
	class TileManager;

	struct ApiScene
	{
		const InputRaw& input;
		const TileManager& tiles;
	};
}