#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

#include "Map.hpp"

namespace nl = nlohmann;
// teraz mozesz pisac np nl::json po prostu

void saveMap(const std::filesystem::path& path, const Map& map)
{
	SPDLOG_INFO("Saved the map to {}", path);
	SPDLOG_ERROR("Failed to save the map to {}", path);
}

Map loadMap(const std::filesystem::path& path)
{
	SPDLOG_INFO("Loaded the map from {}", path);
	SPDLOG_ERROR("Failed to load the map from {}", path);
}