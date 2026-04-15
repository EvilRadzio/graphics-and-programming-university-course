#pragma once

#include <unordered_map>
#include <string>
#include <bitset>
#include <cassert>
#include <optional>
#include <string>

#include <SFML/Graphics.hpp>

namespace px
{
	static constexpr uint32_t k_scanCodeNoUnknownCount = sf::Keyboard::ScancodeCount - 1;
	static constexpr uint32_t k_allButtonsCount = k_scanCodeNoUnknownCount + sf::Mouse::ButtonCount;

	enum class InputId : uint8_t
	{
        A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

        Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9, Num0,

		Enter, Escape, Backspace, Tab, Space, Hyphen, Equal, LBracket, RBracket,
		Backslash, Semicolon, Apostrophe, Grave, Comma, Period, Slash,
		
		F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
		F13, F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24,
		
		CapsLock, PrintScreen, ScrollLock, Pause, Insert, Home, PageUp,
		Delete, End, PageDown, Right, Left, Down, Up,
		
		NumLock, NumpadDivide, NumpadMultiply, NumpadMinus, NumpadPlus, NumpadEqual, NumpadEnter, NumpadDecimal,
		
		Numpad1, Numpad2, Numpad3, Numpad4, Numpad5, Numpad6, Numpad7, Numpad8, Numpad9, Numpad0,
		
		NonUsBackslash, Application, Execute, ModeChange, Help, Menu, Select, Redo, Undo, Cut, Copy, Paste,
		VolumeMute, VolumeUp, VolumeDown, MediaPlayPause, MediaStop, MediaNextTrack, MediaPreviousTrack,
		LControl, LShift, LAlt, LSystem, RControl, RShift, RAlt, System,
		Back, Forward, Refresh, Stop, Search, Favorites, HomePage,
		LaunchApplication1, LaunchApplication2, LaunchMail, LaunchMediaSelect,

		MLeft, MRight, MMiddle, MExtra1, MExtra2
	};

	class Input final
	{
	public:

		bool isPressed(InputId input) const;
		bool isReleased(InputId input) const;
		bool isHeld(InputId input) const;
		std::optional<InputId> getJustPressed() const;
		sf::Vector2i getMousePosition() const;
		sf::Vector2i getMouseDelta() const;

		void newFrame();
		void readEvent(const sf::Event& e);

	private:

		std::bitset<k_allButtonsCount> m_pressed, m_released, m_held;
		sf::Vector2i m_position{};
		sf::Vector2i m_delta{};
		bool m_inWindow{};
	};

	class Mapping final
	{
	public:

		explicit Mapping(const Input& input);

		bool isPressed(const std::string& action) const;
		bool isReleased(const std::string& action) const;
		bool isHeld(const std::string& action) const;

		void set(const std::string& action, InputId input);

	private:

		void bindAssert(const std::string& action) const;

		std::unordered_map<std::string, InputId> m_binds;
		const Input& m_input;
	};

#pragma region InputImplementation

	inline bool Input::isPressed(InputId input) const
	{
		return  m_pressed[static_cast<size_t>(input)];
	}

	inline bool Input::isReleased(InputId input) const
	{
		return m_released[static_cast<size_t>(input)];
	}

	inline bool Input::isHeld(InputId input) const
	{
		return m_held[static_cast<size_t>(input)];
	}

	inline std::optional<InputId> Input::getJustPressed() const
	{
		if (m_pressed.none())
		{
			return {};
		}

		for (size_t i = 0; i < m_pressed.size(); ++i)
		{
			if (m_pressed.test(i))
			{
				return static_cast<InputId>(i);
			}
		}
	}

	inline sf::Vector2i Input::getMousePosition() const
	{
		return m_position;
	}

	inline sf::Vector2i Input::getMouseDelta() const
	{
		return m_delta;
	}

	inline void Input::newFrame()
	{
		m_pressed.reset();
		m_released.reset();
	}

	inline void Input::readEvent(const sf::Event& e)
	{
		if (e.is<sf::Event::FocusLost>())
		{
			m_pressed.reset();
			m_released = m_held;
			m_held.reset();
		}
		else if (e.is<sf::Event::KeyPressed>())
		{
			const auto scan = e.getIf<sf::Event::KeyPressed>();

			if (scan->scancode == sf::Keyboard::Scancode::Unknown) return;

			m_pressed.set(static_cast<size_t>(scan->scancode), true);
			m_held.set(static_cast<size_t>(scan->scancode), true);
		}
		else if (e.is<sf::Event::KeyReleased>())
		{
			const auto scan = e.getIf<sf::Event::KeyReleased>();

			if (scan->scancode == sf::Keyboard::Scancode::Unknown) return;

			m_released.set(static_cast<size_t>(scan->scancode), true);
			m_held.set(static_cast<size_t>(scan->scancode), false);
		}
		else if (e.is<sf::Event::MouseButtonPressed>())
		{
			const auto button = e.getIf<sf::Event::MouseButtonPressed>();

			m_pressed.set(k_scanCodeNoUnknownCount + static_cast<size_t>(button->button), true);
			m_held.set(k_scanCodeNoUnknownCount + static_cast<size_t>(button->button), true);
		}
		else if (e.is<sf::Event::MouseButtonReleased>())
		{
			const auto button = e.getIf<sf::Event::MouseButtonReleased>();

			m_released.set(k_scanCodeNoUnknownCount + static_cast<size_t>(button->button), true);
			m_held.set(k_scanCodeNoUnknownCount + static_cast<size_t>(button->button), false);
		}
		else if (e.is<sf::Event::MouseMoved>())
		{
			const auto move = e.getIf<sf::Event::MouseMoved>();

			m_delta = move->position - m_position;
			m_position = move->position;
		}
		else if (e.is<sf::Event::MouseEntered>())
		{
			m_inWindow = true;
		}
		else if (e.is<sf::Event::MouseLeft>())
		{
			m_inWindow = false;
		}
	}

#pragma endregion InputImplementation
#pragma region MappingImplementation

	inline Mapping::Mapping(const Input& input) :
		m_input(input)
	{}

	inline bool Mapping::isPressed(const std::string& action) const
	{
		bindAssert(action);

		return m_input.isPressed(m_binds.at(action));
	}

	inline bool Mapping::isReleased(const std::string& action) const
	{
		bindAssert(action);

		return m_input.isReleased(m_binds.at(action));
	}

	inline bool Mapping::isHeld(const std::string& action) const
	{
		bindAssert(action);

		return m_input.isHeld(m_binds.at(action));
	}

	inline void Mapping::set(const std::string& action, InputId input)
	{
		m_binds.insert_or_assign(action, input);
	}

	inline void Mapping::bindAssert(const std::string& action) const
	{
		assert(m_binds.count(action) && "A binding needs to be set in px::Mapping prior to requesting");
	}

#pragma endregion MappingImplementation
}