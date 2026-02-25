#include "stdio.h"
#include <SFML/Graphics.hpp>
#include "Game.hpp"
#include "BasicButton.hpp"

// whatever exists above the main function is some bullshit but it's there for testing purposes

struct GameContext
{

};

enum class SceneType
{
	one, two
};

using Scene = gp::Scene<SceneType, GameContext>;

class SceneOne : public Scene
{
public:

	SceneOne()
	{
		sf::RectangleShape shape(sf::Vector2f(200,100));
		shape.setFillColor(sf::Color(0xff0000ff));
		m_button = new gp::BasicButton(shape, sf::Color(0xaa0000ff), [&]() {pushScene(SceneType::two); });
	}

	void update(GameContext& context, const gp::Input& input) override
	{
		m_button->update(input);
	}

	void draw(const GameContext& context, sf::RenderWindow& window) const override
	{
		window.clear(sf::Color::Blue);

		m_button->draw(window);
	}

private:

	gp::BasicButton* m_button;
};

class SceneTwo : public Scene
{
public:

	SceneTwo()
	{
		setToOverlay();

		sf::RectangleShape shape(sf::Vector2f(200, 100));
		shape.setFillColor(sf::Color(0x00ff00ff));
		m_button = new gp::BasicButton(shape, sf::Color(0x00aa00ff), [&]() {popScene(); });
	}

	void update(GameContext& context, const gp::Input& input) override
	{
		m_button->update(input);
	}

	void draw(const GameContext& context, sf::RenderWindow& window) const override
	{
		sf::CircleShape circle(300);
		circle.setFillColor(sf::Color::Magenta);
		window.draw(circle);

		m_button->draw(window);
	}

private:

	gp::BasicButton* m_button;
};

class Game : public gp::Game<SceneType, GameContext>
{
public:

	Game()
	{
		registerScene(SceneType::one, []() {return std::make_unique<SceneOne>(); });
		registerScene(SceneType::two, []() {return std::make_unique<SceneTwo>(); });

		setStartingScene(SceneType::one);
	}
};

int main()
{
	Game game;
	game.run();
}