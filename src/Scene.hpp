#pragma once

#include <optional>

#include "GameContext.hpp"
#include "Input.hpp"

namespace sf
{
	class RenderWindow;
}

namespace gp
{
	template <typename TagEnum>
	class SceneManager;

	template <typename TagEnum>
	class Scene
	{
	public:

		Scene() = default;
		virtual ~Scene() = default;

		virtual void update(GameContext& context, const Input& input) = 0;
		virtual void draw(const GameContext& context, sf::RenderWindow& window) const = 0;

	protected:

		void pushScene(TagEnum tag) { m_push = tag; }
		void popScene() { m_pop = true; }

	private:

		friend SceneManager<TagEnum>;
		std::optional<TagEnum> m_push{};
		bool m_pop{};
	};
}