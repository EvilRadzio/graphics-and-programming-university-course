#pragma once

#include <vector>
#include <memory>
#include <functional>
#include <unordered_map>
#include <assert.h>

#include "Scene.hpp"

namespace Engine
{
	template <Internal I>
	class SceneManager
	{
	public:

		SceneManager() = default;
		~SceneManager() = default;

		using SceneFactory = std::function<std::unique_ptr<Scene<I>>()>;

		void registerScene(typename I::SceneId id, SceneFactory&& factoryFunction) { m_factories[id] = std::move(factoryFunction); }
		void pushScene(typename I::SceneId id) { assert(m_factories.count(id));  m_scenes.push_back(m_factories[id]()); }
		void popScene() { assert(!m_scenes.empty());  m_scenes.pop_back(); }
		bool empty() { return m_scenes.empty(); }

		void updateImgui(typename I::Context& context, UpdateApi& api)
		{
			assert(!m_scenes.empty());

			m_scenes.back()->updateImgui(context, api);
		}

		void update(typename I::Context& context, UpdateApi& api)
		{
			if (m_scenes.back()->m_push)
			{
				typename I::SceneId tag = m_scenes.back()->m_push.value();
				m_scenes.back()->m_push = {};
				pushScene(tag);
			}
			else if (m_scenes.back()->m_pop)
			{
				popScene();
			}

			assert(!m_scenes.empty());

			size_t i = m_scenes.size() - 1;
			for (; i > 0; --i)
			{
				if (!m_scenes[i]->m_isOverlay)
				{
					break;
				}
			}

			for (; i < m_scenes.size(); ++i)
			{
				if (i == m_scenes.size() - 1)
				{
					m_scenes[i]->update(context, api);
					continue;
				}

				Input dummyInput;
				UpdateApi dummyApi{
					dummyInput
				};

				m_scenes[i]->update(context, dummyApi);
			}
		}

		void draw(const typename I::Context& context, DrawApi& api) const
		{
			assert(!m_scenes.empty());

			size_t i = m_scenes.size() - 1;
			for (; i > 0; --i)
			{
				if (!m_scenes[i]->m_isOverlay)
				{
					break;
				}
			}
			
			for (; i < m_scenes.size(); ++i)
			{
				m_scenes[i]->draw(context, api);
			}
		}

	private:

		std::unordered_map<typename I::SceneId, SceneFactory> m_factories;
		std::vector<std::unique_ptr<Scene<I>>> m_scenes;
	};
}