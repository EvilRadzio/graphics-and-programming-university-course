#include "Game/Game.hpp"

int main()
{
	// Putting game on the heap because it will more than likely be a massive class

	Game::Game* game = new Game::Game();
	game->run();
	delete game;
}