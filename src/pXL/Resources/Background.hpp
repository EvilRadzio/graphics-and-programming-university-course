#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

namespace px
{
	class Background;

	class BackgroundData
	{
	public:

		BackgroundData(std::vector<const sf::Texture*>&& textures, const float speedOffset) :
			m_textures(std::move(textures)),
			m_speedOffset(speedOffset)
		{}

	private:

		std::vector<const sf::Texture*> m_textures;
		float m_speedOffset;

		friend Background;
	};

	class Background : public sf::Drawable
	{
	public:

		Background(const BackgroundData& data, const float xPosition) : 
			m_data(data),
			m_xPosition(xPosition)
		{}

	private:

		void draw(sf::RenderTarget& target, sf::RenderStates states) const override
		{
			float xPosition = m_xPosition;
			for (const auto* texture : m_data.m_textures)
			{
				sf::Sprite layer(*texture);
				const sf::FloatRect bounds = layer.getLocalBounds();
				const sf::Vector2f size = static_cast<sf::Vector2f>(target.getSize());
				layer.setScale({ size.y / bounds.size.y, size.y / bounds.size.y});
				layer.setPosition({ xPosition, 0 });

				target.draw(layer);

				xPosition *= m_data.m_speedOffset;
			}
		}

		const BackgroundData& m_data;
		float m_xPosition;

		friend sf::RenderTarget;
	};
}