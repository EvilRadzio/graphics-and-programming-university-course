#pragma once

struct Hitbox
{
	sf::Rect<float> rect{};
};

struct Transform
{
	sf::Vector2f pos{};
	sf::Vector2f vel{};
};

struct Gravity {};

struct Controllable {
	bool canJump{};
};