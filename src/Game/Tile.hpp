#pragma once

#include <string>

struct Tile
{
	enum class Type
	{
		Air, Solid
	};

	Type type{};
	std::string texture{};
	std::string tileName{};
};