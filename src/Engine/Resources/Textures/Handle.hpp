#pragma once

namespace Engine::Resources::Textures
{
	class Manager;

	class Handle
	{
		size_t id{};
		
		Handle(size_t id) : id(id) {};

		friend Manager;
	};
}