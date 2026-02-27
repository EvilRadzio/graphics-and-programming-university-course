#include "Game/Game.hpp"

int main()
{
	// I don't delete the game object because the system will do that and leaving it be makes the game close faster,
	// might not be relevant now but later the game class might be very heavy with a lot of nested elements
	// which would basically force a ton of destructor calls

	Game::Game* game = new Game::Game();
	game->run();
}

// Todo:
//
// - Add a better window default and make it invisible before the run method call
// - Add ImGui support
// - Add a simple ECS
// - Build a simple documentation for what already works