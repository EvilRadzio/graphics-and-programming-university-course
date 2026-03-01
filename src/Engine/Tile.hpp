#pragma once

#include <stdint.h>

namespace Engine
{
	enum class TileType : uint8_t
	{
		solid, air
	};

	struct Tile
	{
		TileType type;
	};
}