#include "Game/Schema/Types.hpp"

namespace Game::Scenes
{
	class TicTacToe : public Schema::Scene
	{
	public:

		enum class T { E, X, O };

		void update(Schema::Context& context, Engine::UpdateApi& api) override
		{
			if (api.input.isPressed(sf::Keyboard::Scancode::Escape))
			{
				popScene();
			}

			sf::Vector2i mousePosition = api.input.getMousePosition();

			for (int32_t y = 0; y < 3; ++y) for (int32_t x = 0; x < 3; ++x)
			{
				sf::Rect<int32_t> rect(
					k_mapStart + sf::Vector2i(x * k_tileSize.x, y * k_tileSize.y),
					k_tileSize
				);

				if (rect.contains(mousePosition))
				{
					if (api.input.isPressed(sf::Mouse::Button::Left))
					{
						m_tiles[y * 3 + x] = T::X;
					}
					else if (api.input.isPressed(sf::Mouse::Button::Right))
					{
						m_tiles[y * 3 + x] = T::O;
					}
				}
			}
		}

		void draw(const Schema::Context& context, Engine::DrawApi& api) const override
		{
			for (int32_t y = 0; y < 3; ++y) for (int32_t x = 0; x < 3; ++x)
			{
				if (m_tiles[y * 3 + x] == T::E) continue;

				sf::RectangleShape rect(static_cast<sf::Vector2f>(k_tileSize));
				rect.setPosition(static_cast<sf::Vector2f>(k_mapStart + sf::Vector2i(x * k_tileSize.x, y * k_tileSize.y)));

				switch (m_tiles[y * 3 + x])
				{
				case T::X:
					rect.setTexture(&api.textures.getTexture("tictactoe/x"));
					break;
				case T::O:
					rect.setTexture(&api.textures.getTexture("tictactoe/o"));
					break;
				}

				api.window.draw(rect);
			}
		}

	private:

		constexpr static sf::Vector2i k_mapStart{ 100, 100 };
		constexpr static sf::Vector2i k_tileSize{ 100, 100 };

		T m_tiles[9];
	};
}