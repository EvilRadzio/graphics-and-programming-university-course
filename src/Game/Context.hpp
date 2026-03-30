#pragma once

#include <unordered_map>

#include "Tile.hpp"

struct Context
{
	std::unordered_map<std::string, Tile> tiles;
};