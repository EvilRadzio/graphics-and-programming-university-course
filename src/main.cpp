#include "Game/Game.hpp"

int main()
{
	Game game;
	game.run();
}

// Todo:
//
// - Add a better window default and make it invisible before the run method call
// - Build a simple documentation for what already works
// - Avoid passing stuff into say a map for convinience, maybe wrap everything later
// - Add a const view to the entity manager