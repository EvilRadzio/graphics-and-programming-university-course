#pragma once

#include <optional>

#include "Internal.hpp"
#include "Input.hpp"
#include "UpdateApi.hpp"
#include "DrawApi.hpp"

namespace sf
{
	class RenderWindow;
}

namespace Engine
{
	template <Internal I>
	class SceneManager;

	// Add input and visual passthrough

	template <Internal I>
	class Scene
	{
	public:

		Scene() = default;
		virtual ~Scene() = default;

		virtual void update(typename I::Context& context, UpdateApi& api) {};
		virtual void draw(const typename I::Context& context, DrawApi& api) const {};

	protected:

		void pushScene(typename I::SceneId tag) { m_push = tag; }
		void popScene() { m_pop = true; }
		void setToOverlay() { m_isOverlay = true; };

	private:

		friend SceneManager<I>;
		std::optional<typename I::SceneId> m_push{};
		bool m_pop{};
		bool m_isOverlay{};
	};
}