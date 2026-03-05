#pragma once

namespace Engine::Tiles
{
	class Manager;
	class TextureManager;

	class Handle
	{
		size_t id{};

		Handle(size_t id) : id(id) {};

		friend Manager;
		friend TextureManager;
	};
}