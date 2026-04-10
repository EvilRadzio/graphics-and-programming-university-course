#pragma once

#include <optional>
#include <any>

#include "ApiDraw.hpp"
#include "ApiScene.hpp"
#include "ApiUpdate.hpp"

namespace sf
{
	class RenderWindow;
}

namespace px
{
	class SceneStack;

	class Scene
	{
	public:

		Scene(const ApiScene& api) : scene(api) {}
		virtual ~Scene() = default;

		virtual void onEnter(std::any&& payload) {}
		virtual void update(ApiUpdate& api) {}
		virtual void fixedUpdate(ApiUpdate& api) {}
		virtual void draw(ApiDraw& api) const = 0;

	protected:

		ApiScene scene;

	private:

		friend SceneStack;
	};
}