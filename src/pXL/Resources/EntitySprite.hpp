#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include <initializer_list>
#include <algorithm>
#include <optional>
#include <cassert>

#include <SFML/Graphics.hpp>

#include "pXL/Manager.hpp"

namespace px
{
	class EntitySprite
	{
	public:

		EntitySprite(const std::string& texture, const sf::IntRect spriteBox, const Manager<sf::Texture>& textures) :
			m_texture(texture),
			m_spriteBox(spriteBox),
			m_gridSize(1, 1),
			m_delay(sf::milliseconds(100)),
			m_textures(textures)
		{}

		bool stateExists(const std::string& name) const { return m_states.count(name); }
		void setGridSize(const sf::Vector2u size) { assert(size.x > 0 && size.y > 0); m_gridSize = size; }
		sf::Vector2u getGridSize() const { return m_gridSize; }
		void setDelay(const sf::Time delay) { assert(delay > sf::Time::Zero); m_delay = delay; }
		sf::Time getDelay() const { return m_delay; }
		void setSpriteBox(const sf::IntRect spriteBox) { m_spriteBox = spriteBox; }
		sf::IntRect getSpriteBox() const { return m_spriteBox; }

		void setState(const std::string& name, std::vector<sf::Vector2u>&& frames)
		{
			assert(std::all_of(frames.begin(), frames.end(), [&](sf::Vector2u part) {
				return part.x < m_gridSize.x && part.y < m_gridSize.y;
			}) && !frames.empty());

			m_states.insert_or_assign(name, std::move(frames));
		}

		void setStateAsDefault(const std::string& name)
		{
			assert(m_states.count(name));
			m_default = name;
		}

		sf::Sprite get(const std::string& name, const sf::Vector2f pixelPosition, const sf::Time elapsed) const
		{
			bool exists = m_states.count(name);

			assert(exists || m_default);

			const sf::Texture& texture = m_textures.get(m_texture);
			sf::Vector2u tileSize{ texture.getSize().x / m_gridSize.x, texture.getSize().y / m_gridSize.y };
			sf::Sprite out(texture);

			const std::vector<sf::Vector2u>& frames = m_states.at(exists? name : m_default.value());
			size_t frame = elapsed.asMilliseconds() / m_delay.asMilliseconds() % frames.size();

			out.setTextureRect(sf::IntRect(
				static_cast<sf::Vector2i>(sf::Vector2u{ frames[frame].x * tileSize.x, frames[frame].y * tileSize.y }),
				static_cast<sf::Vector2i>(tileSize)
			));

			sf::FloatRect bounds = out.getLocalBounds();
			out.setScale({
				static_cast<float>(m_spriteBox.size.x) / bounds.size.x,
				static_cast<float>(m_spriteBox.size.y) / bounds.size.y
			});

			out.setPosition({
				pixelPosition.x + static_cast<float>(m_spriteBox.position.x),
				pixelPosition.y + static_cast<float>(m_spriteBox.position.y)
			});

			return out;
		}

	private:

		std::unordered_map <std::string, std::vector<sf::Vector2u>> m_states;
		std::string m_texture;
		std::optional<std::string> m_default;
		sf::IntRect m_spriteBox;
		sf::Vector2u m_gridSize;
		sf::Time m_delay;
		const Manager<sf::Texture>& m_textures;
	};
}