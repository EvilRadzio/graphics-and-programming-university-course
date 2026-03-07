#pragma once

#include <stack>
#include <memory>
#include <functional>
#include <unordered_map>
#include <assert.h>

#include "Scene.hpp"

namespace px
{
	template <Internal I>
	class SceneManager
	{
	public:

		using SceneFactory = std::function<std::unique_ptr<Scene<I>>()>;

		void registerScene(typename I::SceneId id, SceneFactory&& factoryFunction) { m_factories[id] = std::move(factoryFunction); }
		void pushScene(typename I::SceneId id) { assert(m_factories.count(id));  m_scenes.push(m_factories[id]()); }
		void popScene() { assert(!m_scenes.empty());  m_scenes.pop(); }
		bool empty() { return m_scenes.empty(); }

		void updateGui(typename I::Context& context, ApiUpdateGui& api)
		{
			assert(!m_scenes.empty());

			m_scenes.top()->updateGui(context, api);
		}

		void update(typename I::Context& context, ApiUpdate& api)
		{
			if (m_scenes.top()->m_push)
			{
				typename I::SceneId tag = m_scenes.top()->m_push.value();
				m_scenes.top()->m_push = {};
				pushScene(tag);
			}
			else if (m_scenes.top()->m_pop)
			{
				popScene();
			}

			assert(!m_scenes.empty());

			m_scenes.top()->update(context, api);
		}

		void draw(const typename I::Context& context, ApiDraw& api) const
		{
			assert(!m_scenes.empty());
			
			m_scenes.top()->draw(context, api);
		}

	private:

		std::unordered_map<typename I::SceneId, SceneFactory> m_factories;
		std::stack<std::unique_ptr<Scene<I>>> m_scenes;
	};
}