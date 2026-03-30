#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <utility>
#include <optional>

#include "Clip.hpp"

namespace px
{
	struct SpriteDefinition
	{
		std::unordered_map<std::string, Clip> clips;
		std::string texture;
		sf::IntRect rect;
	};
}