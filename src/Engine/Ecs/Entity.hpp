#pragma once

namespace Engine::Ecs
{
	template <typename ...>
	class EntityManager;

	class Entity
	{
		size_t id{};
		size_t generation{};

		Entity(size_t id, size_t generation) : id(id), generation(generation) {}

		template <typename ...>
		friend class EntityManager;
	};
}