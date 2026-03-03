#pragma once

#include <optional>

#include "Internal.hpp"
#include "Input.hpp"
#include "Apis/Apis.hpp"

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

		virtual ~Scene() = default;

		virtual void updateGui(typename I::Context& context, Apis::UpdateGui& api) = 0;
		virtual void update(typename I::Context& context, Apis::Update& api) = 0;
		virtual void draw(const typename I::Context& context, Apis::Draw& api) const = 0;

	protected:

		void pushScene(typename I::SceneId tag) { m_push = tag; }
		void popScene() { m_pop = true; }

		// Implement pop untill, replace and pop untill replace methods

	private:

		friend SceneManager<I>;
		std::optional<typename I::SceneId> m_push{};
		bool m_pop{};
	};
}