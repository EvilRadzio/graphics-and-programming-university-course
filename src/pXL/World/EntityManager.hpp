#pragma once

#include <vector>
#include <unordered_set>
#include <tuple>
#include <utility>
#include <optional>
#include <array>
#include <cassert>

#include "pXL/SparseSet.hpp"

namespace px
{
	template <typename ... Components>
	class EntityManager
	{
	public:

		static constexpr size_t k_maxEntities = 2000;

		template <typename T>
		using ComponentSet = DataStructures::SparseSet<T, k_maxEntities>;

		class Entity
		{
		public:

			bool exists()
			{
				return m_manager->contains(*this);
			}

			void kill()
			{
				m_manager->kill(*this);
			}

			template <typename C>
			void set(C&& component)
			{
				m_manager->set<C>(*this, std::move(component));
			}

			template <typename C>
			void set(const C& component)
			{
				m_manager->set<C>(*this, component);
			}

			template <typename C, typename ... Args>
			void emplace(Args&&... args)
			{
				m_manager->emplace<C>(*this, std::forward<Args>(args) ...);
			}

			template <typename C>
			void remove()
			{
				m_manager->remove<C>(*this);
			}

			template <typename C>
			bool has() const
			{
				return m_manager->has<C>(*this);
			}

			template <typename C>
			C& get()
			{
				return m_manager->get<C>(*this);
			}

			template <typename C>
			C* tryGet()
			{
				return m_manager->tryGet<C>(*this);
			}

		private:

			Entity(EntityManager* manager, size_t id, size_t generation) :
				m_manager(manager),
				m_id(id),
				m_generation(generation)
			{}

			EntityManager* m_manager;
			size_t m_id{};
			size_t m_generation{};

			friend class EntityManager;
		};

		class Prefab
		{
		public:

			template <typename C>
			void set(C&& component)
			{
				std::get<std::optional<C>>(m_components) = std::move(component);
			}

			template <typename C>
			void set(const C& component)
			{
				std::get<std::optional<C>>(m_components) = component;
			}

			template <typename C, typename ... Args>
			void emplace(Args&&... args)
			{
				std::get<std::optional<C>>(m_components) = C(std::forward<Args>(args) ...);
			}

			template <typename C>
			void remove()
			{
				std::get<std::optional<C>>(m_components) = {};
			}

			template <typename C>
			bool has() const
			{
				return std::get<std::optional<C>>(m_components).has_value();
			}

			template <typename C>
			C& get()
			{
				return std::get<std::optional<C>>(m_components).value();
			}

			template <typename C>
			C* tryGet()
			{
				return has<C>() ? &get<C>() : nullptr;
			}

		private:

			std::tuple<std::optional<Components>...> m_components;

			friend class EntityManager;
		};

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

				Iterator(EntityManager* manager, ComponentSet<T>::View::Iterator iterator)
					: m_manager(manager), m_iterator(iterator)
				{}

				Iterator& operator++()
				{
					++m_iterator;
					return *this;
				}

				Iterator operator++(int)
				{
					Iterator temp = *this;
					++m_iterator;
					return temp;
				}

				Iterator& operator--()
				{
					--m_iterator;
					return *this;
				}

				Iterator operator--(int)
				{
					Iterator temp = *this;
					--m_iterator;
					return temp;
				}

				std::pair<Entity, T&> operator*() const
				{
					auto [id, data] = *m_iterator;
					return std::pair<Entity, T&>{ m_manager->makeEntityFromId(id), data};
				}

				bool operator==(const Iterator& o) const
				{
					return m_iterator == o.m_iterator;
				}

				bool operator!=(const Iterator& o) const
				{
					return !(*this == o);
				}

			private:

				EntityManager* m_manager;
				ComponentSet<T>::View::Iterator m_iterator;
			};

			Iterator begin()
			{
				return Iterator(m_manager, std::get<ComponentSet<T>>(m_manager->m_components).view().begin());
			}

			Iterator end()
			{
				return Iterator(m_manager, std::get<ComponentSet<T>>(m_manager->m_components).view().end());
			}

		private:

			EntityManager* m_manager;
		};

		EntityManager()
		{
			m_generations.fill(0);
			m_freelist.reserve(k_maxEntities);
			for (int32_t id = k_maxEntities - 1; id >= 0; --id)
			{
				m_freelist.push_back(id);
			}
		}

		bool contains(Entity entity) const
		{
			return m_entities.count(entity.m_id) && m_generations[entity.m_id] == entity.m_generation;
		}

		Entity spawn(Prefab&& prefab)
		{
			size_t id = m_freelist.back();
			m_freelist.pop_back();
			m_queued.emplace_back(id, std::move(prefab));
			return Entity(this, id, ++m_generations[id]);
		}

		Entity spawn(const Prefab& prefab)
		{
			size_t id = m_freelist.back();
			m_freelist.pop_back();
			m_queued.emplace_back(id, prefab);
			return Entity(this, id, ++m_generations[id]);
		}

		void kill(Entity entity)
		{
			assert(contains(entity));

			m_killed.push_back(entity.m_id);
		}

		void flush()
		{
			for (const auto id : m_killed)
			{
				(std::get<ComponentSet<Components>>(m_components).tryPop(id), ...);
				m_entities.erase(id);

				m_freelist.push_back(id);
			}
			m_killed.clear();

			for (const auto& [id, prefab] : m_queued)
			{
				((std::get<std::optional<Components>>(prefab.m_components) ?
					std::get<ComponentSet<Components>>(m_components).insert(id, std::get<std::optional<Components>>(prefab.m_components).value()) : (void)0), ...);

				m_entities.insert(id);
			}
			m_queued.clear();
		}

		template <typename C>
		void set(Entity entity, C&& component)
		{
			assert(contains(entity));

			std::get<ComponentSet<C>>(m_components).insertOrAssign(entity.m_id, std::move(component));
		}

		template <typename C>
		void set(Entity entity, const C& component)
		{
			assert(contains(entity));

			std::get<ComponentSet<C>>(m_components).insertOrAssign(entity.m_id, component);
		}

		template <typename C, typename ... Args>
		void emplace(Entity entity, Args&&... args)
		{
			assert(contains(entity));

			std::get<ComponentSet<C>>(m_components).emplace(entity.m_id, std::forward<Args>(args)...);
		}

		template <typename C>
		void remove(Entity entity)
		{
			assert(contains(entity));

			std::get<ComponentSet<C>>(m_components).popIfContains(entity.m_id);
		}

		template <typename C>
		bool has(Entity entity) const
		{
			assert(contains(entity));

			return std::get<ComponentSet<C>>(m_components).contains(entity.m_id);
		}

		template <typename C>
		C& get(Entity entity)
		{
			assert(contains(entity));

			return std::get<ComponentSet<C>>(m_components)[entity.m_id];
		}

		template <typename C>
		C* tryGet(Entity entity)
		{
			return has<C>(entity) ? &std::get<ComponentSet<C>>(m_components)[entity.m_id] : nullptr;
		}

		template<typename C>
		View<C> view()
		{
			return View<C>(this);
		}

		size_t count() const
		{
			return m_entities.size();
		}

		Entity makeEntityFromId(size_t id) {
			assert(m_entities.count(id));

			return Entity(this, id, m_generations[id]);
		}

	private:

		std::tuple<ComponentSet<Components>...>m_components;
		std::unordered_set<size_t> m_entities;
		std::array<size_t, k_maxEntities> m_generations;
		std::vector<size_t> m_freelist;
		std::vector<size_t> m_killed;
		std::vector<std::pair<size_t, Prefab>> m_queued;
	};
}