#include "stdio.h"
#include <SFML/Graphics.hpp>
#include "SceneManager.hpp"

enum class Scene
{
	one, two
};

int main()
{
	gp::SceneManager<Scene> manager;

	gp::Input input;

	constexpr sf::Vector2u windowDimensions{720, 720};

	sf::RenderWindow window(sf::VideoMode(windowDimensions), "Platformer");
	window.setFramerateLimit(60);
	
	while (window.isOpen())
	{
		input.newFrame();

		while (const auto event = window.pollEvent())
		{
			input.readEvent(*event);

			if (event->is<sf::Event::Closed>())
			{
				window.close();
			}
		}

		window.clear(sf::Color::Black);

		window.display();
	}
}