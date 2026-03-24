#pragma once

#include <optional>

#include "Internal.hpp"
#include "ApiDraw.hpp"
#include "ApiScene.hpp"
#include "ApiUpdate.hpp"

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

		Scene(const ApiScene& api, typename I::Context& ctx) : sceneApi(api), ctx(ctx) {}
		virtual ~Scene() = default;

		virtual void update(ApiUpdate& api) = 0;
		virtual void draw(ApiDraw& api) const = 0;

	protected:

		ApiScene sceneApi;
		typename I::Context& ctx;

		void pushScene(typename I::SceneId tag) { m_push = tag; }
		void popScene() { m_pop = true; }

		// Implement pop untill, replace and pop untill replace methods

	private:

		friend SceneManager<I>;
		std::optional<typename I::SceneId> m_push{};
		bool m_pop{};
	};
}