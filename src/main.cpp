#include "Game/Game.hpp"

int main()
{
	Game::Game game;
	game.run();
}

// Todo:
//
// - Add a better window default and make it invisible before the run method call
// - Add ImGui support
// - Add a simple ECS
// - Build a simple documentation for what already works