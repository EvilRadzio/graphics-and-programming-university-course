#pragma once

namespace Engine
{
	class TileManager;
	class TileTextureManager;

	class TileHandle
	{
		size_t id{};

		TileHandle(size_t id) : id(id) {};

		friend TileManager;
		friend TileTextureManager;
	};
}