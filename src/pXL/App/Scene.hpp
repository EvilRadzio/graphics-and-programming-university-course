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
	class SceneStack;

	// Add input and visual passthrough

	template <Internal I>
	class Scene
	{
	public:

		Scene(const ApiScene<I>& api, typename I::Context& ctx) : scene(api), ctx(ctx) {}
		virtual ~Scene() = default;

		virtual void onEnter(const I::ScenePayload* payload) {}
		virtual void update(ApiUpdate& api) = 0;
		virtual void draw(ApiDraw& api) const = 0;

	protected:

		ApiScene<I> scene;
		typename I::Context& ctx;

	private:

		friend SceneStack<I>;
	};
}