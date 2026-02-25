#pragma once

#include <stack>
#include <memory>
#include <functional>
#include <unordered_map>

#include "Scene.hpp"

namespace gp
{
	template <typename TagEnum>
	class SceneManager
	{
	public:

		SceneManager() = default;
		~SceneManager() = default;

		using SceneFactory = std::function<std::unique_ptr<Scene<TagEnum>>()>;

		void registerScene(TagEnum tag, SceneFactory&& factoryFunction) { m_factories[tag] = std::move(factoryFunction); }
		void pushScene(TagEnum tag) { m_scenes.push(m_factories[tag]()); }

		void update(GameContext& context, const Input& input)
		{
			// Todo: add poping and pushing before the update

			m_scenes.top()->update(context, input);
		}

		void draw(const GameContext& context, sf::RenderWindow& window) const
		{
			m_scenes.top()->draw(context, window);
		}

	private:

		std::unordered_map<TagEnum, SceneFactory> m_factories;
		std::stack<std::unique_ptr<Scene<TagEnum>>> m_scenes;
	};
}