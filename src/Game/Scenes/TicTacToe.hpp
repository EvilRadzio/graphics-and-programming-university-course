#pragma once

#include <stack>
#include <array>

#include "Game/Types.hpp"

namespace Scenes
{
	class TicTacToe : public Scene
	{
	public:

		enum class T : uint8_t { E, X, O };

		TicTacToe(ApiScene api, Context& ctx) : Scene(api, ctx)
		{
			std::array<T, 9> empty{ T::E, T::E, T::E, T::E, T::E, T::E, T::E, T::E, T::E };
			m_moves.push(empty);
		}

		void update(px::ApiUpdate& api) override
		{
			{
				ImGuiViewport* viewport = ImGui::GetMainViewport();

				ImGui::SetNextWindowPos(viewport->Pos);
				ImGui::SetNextWindowSize(viewport->Size);

				if (ImGui::Begin("##Menu", nullptr,
					ImGuiWindowFlags_NoDecoration |
					ImGuiWindowFlags_NoMove |
					ImGuiWindowFlags_NoResize |
					ImGuiWindowFlags_NoSavedSettings |
					ImGuiWindowFlags_NoBackground))
				{
					if (ImGui::Button("Go back"))
					{
						scene.comms.pop({});
					}
				}
				ImGui::End();
			}

			if (m_timer > 0)
			{
				--m_timer;

				if (m_timer == 0)
				{
					m_moves = std::stack<std::array<T, 9>>();
					std::array<T, 9> empty{ T::E, T::E, T::E, T::E, T::E, T::E, T::E, T::E, T::E };
					m_moves.push(empty);
					m_winner = T::E;
				}

				return;
			}
			if (scene.input.isPressed(sf::Mouse::Button::Right) &&
				m_moves.size() > 1)
			{
				m_moves.pop();
			}

			sf::Vector2i mousePosition = scene.input.getMousePosition();

			for (int32_t y = 0; y < 3; ++y) for (int32_t x = 0; x < 3; ++x)
			{
				sf::Rect<int32_t> rect(
					k_mapStart + sf::Vector2i(x * (k_tileSize.x + k_spacing), y * (k_tileSize.y + k_spacing)),
					k_tileSize
				);

				if (rect.contains(mousePosition) &&
					scene.input.isPressed(sf::Mouse::Button::Left) &&
					m_moves.top()[y * 3 + x] == T::E)
				{
					m_moves.push(m_moves.top());
					m_moves.top()[y * 3 + x] = m_moves.size() & 1 ? T::X : T::O;
				}
			}

			for (const auto& winning : k_winningCombinations)
			{
				if (!(m_moves.top()[winning.x] == m_moves.top()[winning.y] &&
					m_moves.top()[winning.y] == m_moves.top()[winning.z] &&
					m_moves.top()[winning.x] != T::E))
				{
					continue;
				}

				m_winner = m_moves.top()[winning.x];
				m_timer = k_ticksBeforeRefresh;
				return;
			}

			if (std::none_of(m_moves.top().begin(), m_moves.top().end(), [](T t) {return t == T::E;}))
			{
				m_timer = k_ticksBeforeRefresh;
			}
		}

		void draw(px::ApiDraw& api) const override
		{
			for (int32_t y = 0; y < 3; ++y) for (int32_t x = 0; x < 3; ++x)
			{
				sf::RectangleShape rect(static_cast<sf::Vector2f>(k_tileSize));
				rect.setPosition(static_cast<sf::Vector2f>(
					k_mapStart + sf::Vector2i(x * (k_tileSize.x + k_spacing), y * (k_tileSize.y + k_spacing)
				)));
				rect.setFillColor(sf::Color(0x222222ff));

				api.window.draw(rect);

				rect.setFillColor(sf::Color::White);

				if (m_moves.top()[y * 3 + x] == T::E) continue;

				switch (m_moves.top()[y * 3 + x])
				{
				case T::X:
					rect.setTexture(&api.assets.textures.get("tictactoe/x"));
					break;
				case T::O:
					rect.setTexture(&api.assets.textures.get("tictactoe/o"));
					break;
				}

				api.window.draw(rect);
			}

			if (m_winner == T::E)
			{
				return;
			}

			sf::Text winText(api.assets.font, std::string("The winner is the ")
				+ (m_winner == T::O ? "Circle" : "X"));

			winText.setOrigin(static_cast<sf::Vector2f>(winText.getLocalBounds().size) / 2.0f);
			winText.setPosition(sf::Vector2f(api.window.getSize().x / 2.0f, (api.window.getSize().y / 10) * 9));

			api.window.draw(winText);
		}

	private:

		constexpr static sf::Vector2i k_mapStart{ 190, 190 };
		constexpr static sf::Vector2i k_tileSize{ 100, 100 };
		constexpr static int32_t k_ticksBeforeRefresh{ 120 };
		constexpr static int32_t k_spacing{ 20 };
		constexpr static std::array<sf::Vector3i, 8> k_winningCombinations{
			sf::Vector3i(0, 1, 2),
			sf::Vector3i(3, 4, 5),
			sf::Vector3i(6, 7, 8),
			sf::Vector3i(0, 3, 6),
			sf::Vector3i(1, 4, 7),
			sf::Vector3i(2, 5, 8),
			sf::Vector3i(0, 4, 8),
			sf::Vector3i(2, 4, 6)
		};

		std::stack<std::array<T, 9>> m_moves;
		T m_winner{};
		int32_t m_timer{};
	};
}