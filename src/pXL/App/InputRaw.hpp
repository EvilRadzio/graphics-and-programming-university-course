#pragma once

#include <bitset>
#include <cassert>
#include <optional>

#include <SFML/Window.hpp>

namespace px
{
	// Inside of this class the keayboard input scancodes are stored as scancode + 1
	// The reason for that is SFML using -1 to represent an unknown key
	// Maybe this kind of input should just be ignored, I'll think about it

	class InputRaw
	{
	public:
		
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
			if (e.is<sf::Event::FocusLost>())
			{
				m_keyboardHeld.reset();
				m_keyboardPressed.reset();
				m_keyboardReleased.reset();
				m_mouseHeld.reset();
				m_mousePressed.reset();
				m_mouseReleased.reset();
			}
			else if (e.is<sf::Event::KeyPressed>())
			{
				const auto scan = e.getIf<sf::Event::KeyPressed>();

				m_keyboardPressed.set(static_cast<size_t>(scan->scancode) + 1, true);
				m_keyboardHeld.set(static_cast<size_t>(scan->scancode) + 1, true);
			}
			else if (e.is<sf::Event::KeyReleased>())
			{
				const auto scan = e.getIf<sf::Event::KeyReleased>();

				m_keyboardReleased.set(static_cast<size_t>(scan->scancode) + 1, true);
				m_keyboardHeld.set(static_cast<size_t>(scan->scancode) + 1, false);
			}
			else if (e.is<sf::Event::MouseButtonPressed>())
			{
				const auto button = e.getIf<sf::Event::MouseButtonPressed>();

				m_mousePressed.set(static_cast<size_t>(button->button), true);
				m_mouseHeld.set(static_cast<size_t>(button->button), true);
			}
			else if (e.is<sf::Event::MouseButtonReleased>())
			{
				const auto button = e.getIf<sf::Event::MouseButtonReleased>();

				m_mouseReleased.set(static_cast<size_t>(button->button), true);
				m_mouseHeld.set(static_cast<size_t>(button->button), false);
			}
			else if (e.is<sf::Event::MouseMoved>())
			{
				const auto move = e.getIf<sf::Event::MouseMoved>();

				m_mousePosition = move->position;
			}
			else if (e.is<sf::Event::MouseEntered>())
			{
				m_mouseInWindow = true;
			}
			else if (e.is<sf::Event::MouseLeft>())
			{
				m_mouseInWindow = false;
			}
		}

		void newTick()
		{
			m_keyboardPressed.reset();
			m_keyboardReleased.reset();

			m_mousePressed.reset();
			m_mouseReleased.reset();
		}

	private:

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