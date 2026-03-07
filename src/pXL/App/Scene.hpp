#pragma once

#include <optional>

#include "Internal.hpp"
#include "ApiDraw.hpp"
#include "ApiScene.hpp"
#include "ApiUpdate.hpp"
#include "ApiUpdateGui.hpp"

namespace sf
{
	class RenderWindow;
}

namespace px
{
	template <Internal I>
	class SceneManager;

	// Add input and visual passthrough

	template <Internal I>
	class Scene
	{
	public:

		Scene(const ApiScene& api) : sceneApi(api) {}
		virtual ~Scene() = default;

		virtual void updateGui(typename I::Context& context, ApiUpdateGui& api) = 0;
		virtual void update(typename I::Context& context, ApiUpdate& api) = 0;
		virtual void draw(const typename I::Context& context, ApiDraw& api) const = 0;

	protected:

		ApiScene sceneApi;

		void pushScene(typename I::SceneId tag) { m_push = tag; }
		void popScene() { m_pop = true; }

		// Implement pop untill, replace and pop untill replace methods

	private:

		friend SceneManager<I>;
		std::optional<typename I::SceneId> m_push{};
		bool m_pop{};
	};
}