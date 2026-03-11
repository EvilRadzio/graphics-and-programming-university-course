#pragma once

#include <vector>
#include <string>
#include <utility>

#include "TileManager.hpp"

namespace px::Load
{
	std::vector<std::pair<TileHandle, std::string>> tilesRecursive(TileManager& tiles, const std::string& directoryPath);
}