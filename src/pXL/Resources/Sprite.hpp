#pragma once

#include <unordered_map>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include "Clip.hpp"

namespace px
{
	class Sprite;

	class SpriteData
	{
	public:

		AnimatedSpriteData(const sf::Texture& texture) :
			m_texture(texture) {}

		void setSpriteBox(const sf::FloatRect rect) { m_rect = rect; }
		sf::FloatRect getSpriteRect() const { return m_rect; }

	private:

		std::unordered_map<std::string, Clip> m_clips;
		sf::FloatRect m_rect{ { -0.5f, -0.5f }, { 1.0f, 1.0f } };
		const sf::Texture& m_texture;

		friend Sprite;
	};

	class Sprite : public sf::Transformable, public sf::Drawable
	{

	};
}