#pragma once

#include <vector>
#include <unordered_set>
#include <tuple>
#include <utility>

#include "pXL/SparseSet.hpp"
#include "Entity.hpp"

namespace px
{
	template <typename ... Components>
	class EntityManager
	{
	private:

		static constexpr size_t k_maxEntities = 2000;

		template <typename T>
		using ComponentSet = DataStructures::SparseSet<T, k_maxEntities>;

		template <typename T>
		class View
		{
		public:

			View(EntityManager* manager)
				: m_manager(manager) {
			}

			class Iterator
			{
			public:

				Iterator(EntityManager* manager, ComponentSet<T>::Iterator setIterator)
					: m_manager(manager), m_iterator(setIterator) {
				}

				Iterator& operator++()
				{
					++m_iterator;
					return *this;
				}

				std::pair<Entity, T&> operator*() const
				{
					return std::pair<Entity, T&>{ m_manager->makeEntityFromId(m_iterator.index()), m_iterator.data()};
				}

				bool operator!=(const Iterator& other) const
				{
					return m_iterator != other.m_iterator;
				}

			private:

				EntityManager* m_manager;
				ComponentSet<T>::Iterator m_iterator;
			};

			Iterator begin()
			{
				return Iterator(m_manager, std::get<ComponentSet<T>>(m_manager->m_components).begin());
			}

			Iterator end()
			{
				return Iterator(m_manager, std::get<ComponentSet<T>>(m_manager->m_components).end());
			}

		private:

			EntityManager* m_manager;
		};

	public:

		EntityManager() : m_generations(k_maxEntities, 0)
		{
			m_freelist.reserve(k_maxEntities);
			for (int32_t id = k_maxEntities - 1; id >= 0; --id)
			{
				m_freelist.push_back(id);
			}
		}

		Entity spawn()
		{
			size_t id = m_freelist.back();
			m_freelist.pop_back();
			m_entities.insert(id);
			return Entity(id, ++m_generations[id]);
		}

		void kill(Entity entity)
		{
			assert(m_entities.count(entity.id) && m_generations[entity.id] == entity.generation);

			m_killed.push_back(entity.id);
		}

		void despawn()
		{
			for (const auto id : m_killed)
			{
				(std::get<ComponentSet<Components>>(m_components).popIfContains(id), ...);
				m_entities.erase(id);

				m_freelist.push_back(id);
			}

			m_killed.clear();
		}

		template <typename C, typename ... Args>
		void add(Entity entity, Args&&... args)
		{
			assert(m_entities.count(entity.id) && m_generations[entity.id] == entity.generation);

			std::get<ComponentSet<C>>(m_components).emplace(entity.id, std::forward<Args>(args)...);
		}

		template <typename C>
		void remove(Entity entity)
		{
			assert(m_entities.count(entity.id) && m_generations[entity.id] == entity.generation);

			std::get<ComponentSet<C>>(m_components).popIfContains(entity.id);
		}

		template <typename C>
		bool has(Entity entity)
		{
			assert(m_entities.count(entity.id) && m_generations[entity.id] == entity.generation);

			return std::get<ComponentSet<C>>(m_components).contains(entity.id);
		}

		template <typename C>
		C& get(Entity entity)
		{
			assert(m_entities.count(entity.id) && m_generations[entity.id] == entity.generation);

			return std::get<ComponentSet<C>>(m_components)[entity.id];
		}

		template<typename C>
		View<C> view()
		{
			return View<C>(this);
		}

		bool contains(Entity entity)
		{
			return m_entities.count(entity.id) && m_generations[entity.id] == entity.generation;
		}

		Entity makeEntityFromId(size_t id) {
			assert(m_entities.count(id));

			return Entity(id, m_generations[id]);
		}

	private:

		std::vector<size_t> m_generations;
		std::vector<size_t> m_freelist;
		std::vector<size_t> m_killed;
		std::unordered_set<size_t> m_entities;
		std::tuple<ComponentSet<Components>...>m_components;
	};
}