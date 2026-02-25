#pragma once

#include <optional>

#include "Input.hpp"

namespace sf
{
	class RenderWindow;
}

namespace gp
{
	template <typename SceneTagEnum, typename ContextObject>
	class SceneManager;

	// Add input and visual passthrough

	template <typename SceneTagEnum, typename ContextObject>
	class Scene
	{
	public:

		Scene() = default;
		virtual ~Scene() = default;

		virtual void update(ContextObject& context, const Input& input) = 0;
		virtual void draw(const ContextObject& context, sf::RenderWindow& window) const = 0;

	protected:

		void pushScene(SceneTagEnum tag) { m_push = tag; }
		void popScene() { m_pop = true; }
		void setToOverlay() { m_isOverlay = true; };

	private:

		friend SceneManager<SceneTagEnum, ContextObject>;
		std::optional<SceneTagEnum> m_push{};
		bool m_pop{};
		bool m_isOverlay{};
	};
}