#pragma once

namespace px
{
	class TextureManager;

	class TextureHandle
	{
		size_t id{};
		
		TextureHandle(size_t id) : id(id) {};

		friend TextureManager;
	};
}