#pragma once

#include <vector>
#include <memory>
#include <functional>
#include <unordered_map>
#include <assert.h>

#include "Scene.hpp"

namespace gp
{
	template <typename SceneTagEnum, typename ContextObject>
	class SceneManager
	{
	public:

		SceneManager() = default;
		~SceneManager() = default;

		using SceneFactory = std::function<std::unique_ptr<Scene<SceneTagEnum, ContextObject>>()>;

		void registerScene(SceneTagEnum tag, SceneFactory&& factoryFunction) { m_factories[tag] = std::move(factoryFunction); }
		void pushScene(SceneTagEnum tag) { assert(m_factories.count(tag));  m_scenes.push_back(m_factories[tag]()); }
		void popScene() { assert(!m_scenes.empty());  m_scenes.pop_back(); }
		bool empty() { return m_scenes.empty(); }

		void update(ContextObject& context, const Input& input)
		{
			if (m_scenes.back()->m_push)
			{
				SceneTagEnum tag = m_scenes.back()->m_push.value();
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
				m_scenes[i]->update(context, i == m_scenes.size() - 1 ? input : Input());
			}
		}

		void draw(const ContextObject& context, sf::RenderWindow& window) const
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
				m_scenes[i]->draw(context, window);
			}
		}

	private:

		std::unordered_map<SceneTagEnum, SceneFactory> m_factories;
		std::vector<std::unique_ptr<Scene<SceneTagEnum, ContextObject>>> m_scenes;
	};
}