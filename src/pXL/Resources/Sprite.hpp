#pragma once

#include <unordered_map>
#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Transformable.hpp>

namespace px
{
	struct Frame
	{
		sf::IntRect rect;
		sf::Time time;
	};

	class Sprite;

	class Clip
	{
	public:

		Clip(std::vector<Frame>&& frames) :
			m_frames(std::move(frames))
		{
			for (auto& frame : m_frames)
			{
				frame.time = std::max(frame.time, sf::milliseconds(10));
				m_animationTime += frame.time;
			}
		}

		void setLooping(bool looping) { m_looping = looping; }
		bool looping() const { return m_looping; }

	private:

		bool clipEnded(const sf::Time elapsed) const
		{
			return !m_looping && m_animationTime <= elapsed;
		}

		sf::IntRect getFrameRect(const sf::Time elapsed) const
		{
			sf::Time into = elapsed;

			if (m_looping)
			{
				into = sf::milliseconds(
					elapsed.asMilliseconds() % m_animationTime.asMilliseconds()
				);
			}
			else if (elapsed >= m_animationTime)
			{
				return m_frames.back().rect;
			}

			for (const auto& frame : m_frames)
			{
				if (into < frame.time)
					return frame.rect;
				into -= frame.time;
			}

			return m_frames.back().rect;
		}

		std::vector<Frame> m_frames;
		sf::Time m_animationTime{};
		bool m_looping{};

		friend Sprite;
	};

	class SpriteData
	{
	public:

		SpriteData(const sf::Texture& texture) :
			m_texture(&texture) {}

		void setClip(const std::string& name, Clip&& clip)
		{
			m_clips.insert_or_assign(name, std::move(clip));
		}

	private:

		std::unordered_map<std::string, Clip> m_clips;
		const sf::Texture* m_texture;

		friend Sprite;
	};

	class Sprite : public sf::Drawable, public sf::Transformable
	{
	public:

		Sprite(const SpriteData& spriteData, const std::string& state, const sf::Time elapsed) :
			m_state(state),
			m_spriteData(spriteData),
			m_elapsed(elapsed)
		{}

		void setMirrored(bool mirrored) { m_mirrored = mirrored; }
		bool isMirrored() const { return m_mirrored; }

	private:

		void draw(sf::RenderTarget& target, sf::RenderStates states) const override
		{
			if (!m_spriteData.m_clips.count(m_state))
			{
				return;
			}

			sf::Sprite sprite(*m_spriteData.m_texture);
			sf::IntRect rect = m_spriteData.m_clips.at(m_state).getFrameRect(m_elapsed);

			if (m_mirrored)
			{
				int32_t width = rect.size.x;
				rect.size.x = -width;
				rect.position.x += width;
			}

			sprite.setTextureRect(rect);
			sprite.setScale(getScale());
			sprite.setPosition(getPosition());
			sprite.setOrigin(getOrigin());

			target.draw(sprite);
		}

		const std::string m_state;
		const SpriteData& m_spriteData;
		const sf::Time m_elapsed;
		bool m_mirrored{};

		friend sf::RenderTarget;
	};
}