#pragma once

#include <bitset>
#include <cassert>
#include <optional>

#include <SFML/Window.hpp>

namespace Engine
{
	// Add bindings

	class Input
	{
	public:

		Input() = default;
		~Input() = default;

		bool isHeld(sf::Keyboard::Scan scan) const { return m_keyboardHeld[static_cast<size_t>(scan) + 1]; }
		bool isPressed(sf::Keyboard::Scan scan) const { return m_keyboardPressed[static_cast<size_t>(scan) + 1]; }
		bool isReleased(sf::Keyboard::Scan scan) const { return m_keyboardReleased[static_cast<size_t>(scan) + 1]; }
		bool isHeld(sf::Mouse::Button button) const { return m_mouseHeld[static_cast<size_t>(button)]; }
		bool isPressed(sf::Mouse::Button button) const { return m_mousePressed[static_cast<size_t>(button)]; }
		bool isReleased(sf::Mouse::Button button) const { return m_mouseReleased[static_cast<size_t>(button)]; }
		bool isAnyPressed() const { return m_mousePressed.any() || m_keyboardPressed.any(); }
		bool isMouseInWindow() const { return m_mouseInWindow; }
		sf::Vector2i getMousePosition() const { return m_mousePosition; }

		std::optional<sf::Keyboard::Key> justPressedKey() {
			if (m_keyboardPressed.none())
			{
				return {};
			}

			for (size_t i = 0; i < m_keyboardPressed.size(); ++i)
			{
				if (m_keyboardPressed.test(i))
				{
					return static_cast<sf::Keyboard::Key>(i - 1);
				}
			}
		}

		std::optional<sf::Mouse::Button> justPressedButton() {
			if (m_mousePressed.none())
			{
				return {};
			}

			for (size_t i = 0; i < m_keyboardPressed.size(); ++i)
			{
				if (m_mousePressed.test(i))
				{
					return static_cast<sf::Mouse::Button>(i);
				}
			}
		}

		void readEvent(const sf::Event& e)
		{
			if (e.is<sf::Event::KeyPressed>())
			{
				const auto scan = e.getIf<sf::Event::KeyPressed>();
				registerPress(scan);
			}
			else if (e.is<sf::Event::KeyReleased>())
			{
				const auto scan = e.getIf<sf::Event::KeyReleased>();
				registerRelease(scan);
			}
			else if (e.is<sf::Event::MouseButtonPressed>())
			{
				const auto button = e.getIf<sf::Event::MouseButtonPressed>();
				registerPress(button);
			}
			else if (e.is<sf::Event::MouseButtonReleased>())
			{
				const auto button = e.getIf<sf::Event::MouseButtonReleased>();
				registerRelease(button);
			}
			else if (e.is<sf::Event::MouseMoved>())
			{
				const auto move = e.getIf<sf::Event::MouseMoved>();
				registerMove(move);
			}
			else if (e.is<sf::Event::MouseEntered>())
			{
				registerMouseInWindow(true);
			}
			else if (e.is<sf::Event::MouseLeft>())
			{
				registerMouseInWindow(false);
			}
		}

		void newFrame()
		{
			m_keyboardPressed.reset();
			m_keyboardReleased.reset();

			m_mousePressed.reset();
			m_mouseReleased.reset();
		}

	private:

		void registerMove(const sf::Event::MouseMoved* const move)
		{
			m_mousePosition = move->position;
		}

		void registerMouseInWindow(bool present)
		{
			m_mouseInWindow = present;
		}

		void registerPress(const sf::Event::KeyPressed* const scan)
		{
			m_keyboardPressed.set(static_cast<size_t>(scan->scancode) + 1, true);
			m_keyboardHeld.set(static_cast<size_t>(scan->scancode) + 1, true);
		}

		void registerRelease(const sf::Event::KeyReleased* const scan)
		{
			m_keyboardReleased.set(static_cast<size_t>(scan->scancode) + 1, true);
			m_keyboardHeld.set(static_cast<size_t>(scan->scancode) + 1, false);
		}

		void registerPress(const sf::Event::MouseButtonPressed* const button)
		{
			m_mousePressed.set(static_cast<size_t>(button->button), true);
			m_mouseHeld.set(static_cast<size_t>(button->button), true);
		}

		void registerRelease(const sf::Event::MouseButtonReleased* const button)
		{
			m_mouseReleased.set(static_cast<size_t>(button->button), true);
			m_mouseHeld.set(static_cast<size_t>(button->button), false);
		}
		
		std::bitset<sf::Keyboard::ScancodeCount> m_keyboardHeld;
		std::bitset<sf::Keyboard::ScancodeCount> m_keyboardPressed;
		std::bitset<sf::Keyboard::ScancodeCount> m_keyboardReleased;

		std::bitset<sf::Mouse::ButtonCount> m_mouseHeld;
		std::bitset<sf::Mouse::ButtonCount> m_mousePressed;
		std::bitset<sf::Mouse::ButtonCount> m_mouseReleased;

		sf::Vector2i m_mousePosition{};
		bool m_mouseInWindow{};
	};
}