#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

#include "Map.hpp"

namespace nl = nlohmann;
// teraz mozesz pisac np nl::json po prostu

void saveMap(const std::filesystem::path& path, const Map& map)
{
	// Uzyj tych macro logujacych by ladnie bylo w konsoli widac co sie dzieje

	//SPDLOG_INFO("Saved the map to {}", path);
	//SPDLOG_ERROR("Failed to save the map to {}", path);
}

Map loadMap(const std::filesystem::path& path)
{
	// Mialo by sens pusta mape w jakis wymiarach dac jesli cos erroruje
	// bo w grze lepiej by sie pokazala mapa ktora ma napis ze cos poszlo nie tak niz by wywalilo gre

	//SPDLOG_INFO("Loaded the map from {}", path);
	//SPDLOG_ERROR("Failed to load the map from {}", path);
}