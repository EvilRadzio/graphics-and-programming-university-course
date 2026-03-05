#pragma once

namespace Engine::World::Ecs
{
	template <typename ...>
	class Manager;

	class Entity
	{
		size_t id{};
		size_t generation{};

		Entity(size_t id, size_t generation) : id(id), generation(generation) {}

		template <typename ...>
		friend class Manager;
	};
}