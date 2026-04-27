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

	class SpritePrefab
	{
	public:

		SpritePrefab(const sf::Texture& texture) :
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

	class Animation
	{
	public:

		Animation() = default;

		Animation(const std::string& animation) :
			m_animation(animation)
		{}

		Animation(const std::string& animation, sf::Time animationElapsed) :
			m_animation(animation),
			m_animationElapsed(animationElapsed)
		{}

		void startAnimation(const std::string& animation)
		{
			m_animation = animation;
			m_animationElapsed = sf::Time::Zero;
		}

		void animate(const std::string& animation)
		{
			if (m_animation != animation)
			{
				m_animationElapsed = sf::Time::Zero;
			}
			m_animation = animation;
		}

		void setMirrored(bool mirrored) {
			m_mirrored = mirrored;
		}

		bool isMirrored() const {
			return m_mirrored;
		}

		void update(const sf::Time variableDt)
		{
			m_animationElapsed += variableDt;
		}

	private:

		std::string m_animation;
		sf::Time m_animationElapsed;
		bool m_mirrored{};

		friend Sprite;
	};

	class Sprite : public sf::Drawable, public sf::Transformable
	{
	public:

		Sprite(const SpritePrefab& spritePrefab, const Animation& spriteInfo, const sf::Time elapsed) :
			m_spritePrefab(spritePrefab),
			m_spriteInfo(spriteInfo),
			m_elapsed(elapsed)
		{}

	private:

		void draw(sf::RenderTarget& target, sf::RenderStates states) const override
		{
			if (!m_spritePrefab.m_clips.count(m_spriteInfo.m_animation))
			{
				return;
			}

			sf::Sprite sprite(*m_spritePrefab.m_texture);
			bool looping = m_spritePrefab.m_clips.at(m_spriteInfo.m_animation).looping();
			sf::IntRect rect = m_spritePrefab.m_clips.at(m_spriteInfo.m_animation).getFrameRect(looping? m_elapsed : m_spriteInfo.m_animationElapsed);

			if (m_spriteInfo.m_mirrored)
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

		const SpritePrefab& m_spritePrefab;
		const Animation& m_spriteInfo;
		sf::Time m_elapsed;

		friend sf::RenderTarget;
	};
}