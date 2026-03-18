#pragma once

namespace px
{
	class TileManager;
	class TileTextureManager;

	struct TileHandle
	{
		size_t id{};

		TileHandle(size_t id) : id(id) {};

		friend TileManager;
		friend TileTextureManager;
	};
}