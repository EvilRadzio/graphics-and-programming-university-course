#pragma once

#include <functional>

#include <SFML/Graphics.hpp>

#include "Input.hpp"

namespace gp
{
	// Needs a slight rework + needs to display text
	
	class BasicButton
	{
	public:

		BasicButton(sf::RectangleShape shape, sf::Color hoverColor, std::function<void()>&& callback) :
			m_shape(shape), m_hoverColor(hoverColor), m_callback(callback) {}

		void update(const Input& input)
		{
			sf::Vector2f mousePosition = static_cast<sf::Vector2f>(input.getMousePosition());
			sf::Vector2f topLeft = m_shape.getPosition();
			sf::Vector2f bottomRight = topLeft + m_shape.getSize();

			m_hovered = mousePosition.x >= topLeft.x && mousePosition.y > topLeft.y &&
				mousePosition.x <= bottomRight.x && mousePosition.y <= bottomRight.y;

			if (input.isPressed(sf::Mouse::Button::Left) && m_hovered)
			{
				m_callback();
			}
		}

		void draw(sf::RenderWindow& window) const
		{
			if (m_hovered)
			{
				sf::RectangleShape hoveredShape = m_shape;;
				hoveredShape.setFillColor(m_hoverColor);

				window.draw(hoveredShape);
				return;
			}

			window.draw(m_shape);
		}

	private:

		std::function<void()> m_callback;
		sf::RectangleShape m_shape;
		sf::Color m_hoverColor;
		bool m_hovered{};
	};
}