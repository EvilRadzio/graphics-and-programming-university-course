#pragma once

#include <vector>
#include <memory>
#include <functional>
#include <unordered_map>
#include <assert.h>

#include "Scene.hpp"

namespace px
{
	template <Internal I>
	class SceneStack
	{
	public:

		using SceneFactory = std::function<std::unique_ptr<Scene<I>>()>;

		void registerScene(typename I::SceneId id, SceneFactory&& factoryFunction) { m_factories[id] = std::move(factoryFunction); }
		void pushScene(typename I::SceneId id) { assert(m_factories.count(id));  m_scenes.push_back(m_factories[id]()); m_sceneIds.push_back(id); }
		void popScene() { assert(!m_scenes.empty());  m_scenes.pop_back(); m_sceneIds.pop_back(); }
		bool empty() { return m_scenes.empty(); }

		void update(ApiUpdate& api, SceneComms<I>& comms)
		{
			if (comms.m_action == SceneComms<I>::Action::Push)
			{
				const typename I::SceneId requested = comms.m_requested.value();
				comms.m_action = SceneComms<I>::Action::None;
				pushScene(requested);
			}
			else if (comms.m_action == SceneComms<I>::Action::Pop)
			{
				comms.m_action = SceneComms<I>::Action::None;
				popScene();
			}
			else if (comms.m_action == SceneComms<I>::Action::PopUntil)
			{
				const typename I::SceneId requested = comms.m_requested.value();
				comms.m_action = SceneComms<I>::Action::None;
				
				while (m_sceneIds.back() != requested)
				{
					popScene();
				}
			}

			assert(!m_scenes.empty());

			m_scenes.back()->update(api);
		}

		void draw(ApiDraw& api) const
		{
			assert(!m_scenes.empty());
			
			const size_t count = m_scenes.size();
			size_t firstRenderable = count;
			for (size_t i = count - 1; i < count; --i)
			{
				firstRenderable = i;

				if (!m_scenes[i]->getProperties().renderThrough)
				{
					break;
				}
			}

			for (size_t i = firstRenderable; i < count; ++i)
			{
				m_scenes[i]->draw(api);
			}
		}

	private:

		std::unordered_map<typename I::SceneId, SceneFactory> m_factories;
		std::vector<std::unique_ptr<Scene<I>>> m_scenes;
		std::vector<typename I::SceneId> m_sceneIds;

		// ids really shouldn't be in a separate vector
	};
}