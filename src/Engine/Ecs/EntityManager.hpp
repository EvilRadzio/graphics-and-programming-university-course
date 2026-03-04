#pragma once

#include <vector>
#include <unordered_set>
#include <tuple>
#include "DataStructures/SparseSet.hpp"

namespace Engine::Ecs
{
	template <typename ... Args>
	class EntityManager
	{
	public:

		static constexpr size_t k_maxEntities = 2000;

		template <typename T>
		using ComponentSet = DataStructures::SparseSet<T, k_maxEntities;

		EntityManager()
		{
			m_freelist.reserve(k_MaxEntities);
			for (int32_t id = k_MaxEntities - 1; id >= 0; --id)
			{
				m_freelist.push_back(id);
			}
		}

		size_t spawn()
		{
			size_t id = m_freelist.back();
			m_freelist.pop_back();
			m_entities.insert(id);
			return id;
		}

		void kill(size_t id)
		{

		}

		void despawn(size_t id)
		{
			if (!m_entities.count(id)) return;

			std::get<ComponentSet<Comp::Collider>>(m_components).popIfContains(id);
			std::get<ComponentSet<Comp::Controlable>>(m_components).popIfContains(id);
			std::get<ComponentSet<Comp::Transform>>(m_components).popIfContains(id);
			std::get<ComponentSet<Comp::Velocity>>(m_components).popIfContains(id);

			m_freelist.push_back(id);
		}

		template <typename C, typename ... Args>
		void add(size_t id, Args&&... args)
		{
			std::get<ComponentSet<C>>(m_components).emplace(id, std::forward<Args>(args)...);
		}

		template <typename C>
		void remove(size_t id)
		{
			std::get<ComponentSet<C>>(m_components).popIfContains(id);
		}

		template <typename C>
		bool has(size_t id)
		{
			return std::get<ComponentSet<C>>(m_components).contains(id);
		}

		template <typename C>
		C& get(size_t id)
		{
			return std::get<ComponentSet<C>>(m_components)[id];
		}

		template<typename C>
		auto& getSet()
		{
			return std::get<ComponentSet<C>>(m_components);
		}

		bool contains(size_t id)
		{
			assert(id < k_MaxEntities, "You're trying to check if an entity of impossible id exists");

			return m_entities.count(id);
		}

	private:

		std::vector<size_t> m_freelist;
		std::unordered_set<size_t> m_entities;
		std::tuple<ComponentSet<Args>>...>m_components;
	};
}