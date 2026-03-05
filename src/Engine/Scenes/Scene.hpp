#pragma once

#include <optional>

#include "Internal.hpp"
#include "Apis/Apis.hpp"

namespace sf
{
	class RenderWindow;
}

namespace Engine::Scenes
{
	template <Internal I>
	class Manager;

	// Add input and visual passthrough

	template <Internal I>
	class Scene
	{
	public:

		Scene(const Engine::Apis::Scene& api) : sceneApi(api) {}
		virtual ~Scene() = default;

		virtual void updateGui(typename I::Context& context, Apis::UpdateGui& api) = 0;
		virtual void update(typename I::Context& context, Apis::Update& api) = 0;
		virtual void draw(const typename I::Context& context, Apis::Draw& api) const = 0;

	protected:

		Engine::Apis::Scene sceneApi;

		void pushScene(typename I::SceneId tag) { m_push = tag; }
		void popScene() { m_pop = true; }

		// Implement pop untill, replace and pop untill replace methods

	private:

		friend Manager<I>;
		std::optional<typename I::SceneId> m_push{};
		bool m_pop{};
	};
}