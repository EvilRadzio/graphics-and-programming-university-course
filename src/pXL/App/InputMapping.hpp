#pragma once

#include <type_traits>
#include <array>

#include "InputRaw.hpp"

namespace px
{
	template <typename Action, size_t k_maxActions>
		requires std::is_enum_v<Action>
	class InputMapping
	{
	public:

		InputMapping(const InputRaw& raw)
		{
			for (auto& bind : m_bindings)
			{
				bind = sf::Keyboard::Scancode::Unknown;
			}
		}

		bool isHeld(Action action) { return m_raw.isHeld(m_bindings[static_cast<size_t>(action)]); }
		bool isPressed(Action action) { return m_raw.isPressed(m_bindings[static_cast<size_t>(action)]); }
		bool isReleased(Action action) { return m_raw.isReleased(m_bindings[static_cast<size_t>(action)]); }

		void set(Action action, sf::Keyboard::Scancode scancode)
		{
			m_bindings[static_cast<size_t>(action)] = scancode;
		}

	private:

		std::array<sf::Keyboard::Scancode, k_maxActions> m_bindings;
		const InputRaw& m_raw;
	};
}