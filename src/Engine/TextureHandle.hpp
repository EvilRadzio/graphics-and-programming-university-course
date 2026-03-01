#pragma once

namespace Engine
{
	class TextureManager;

	class TextureHandle
	{
	public:

		~TextureHandle() = default;

	private:

		size_t id{};
		
		TextureHandle(size_t id) : id(id) {};

		friend TextureManager;
	};
}