#pragma once

#include <algorithm>

#include <SFML/Graphics.hpp>

namespace px
{
	struct Frame
	{
		sf::IntRect rect;
		sf::Time time;
	};

	class Clip
	{
	public:

		Clip(bool looping, std::vector<Frame>&& frames) :
			m_frames(std::move(frames)),
			m_looping(looping)
		{
			for (auto& frame : m_frames)
			{
				frame.time = std::max(frame.time, sf::milliseconds(10));
				m_animationTime += frame.time;
			}
		}

		bool looping() const { return m_looping; }

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

	private:

		std::vector<Frame> m_frames;
		sf::Time m_animationTime{};
		bool m_looping;
	};
}