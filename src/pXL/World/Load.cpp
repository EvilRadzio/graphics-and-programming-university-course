#include "Load.hpp"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

std::vector<std::pair<px::TileHandle, std::string>> px::Load::tilesRecursive(px::TileManager& tiles, const std::string& directoryPath)
{
	return{};
}