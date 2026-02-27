#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <assert.h>
#include <filesystem>

#include <SFML/Graphics.hpp>

namespace Engine
{
	// Make like internal struct tha can be passed to everything

	class ResourceManager;

	class TextureHandle
	{
	public:

		~TextureHandle() = default;

		size_t getId() { return id; }

	private:

		TextureHandle(size_t id) : id(id) {};

		friend ResourceManager;
		size_t id{};
	};

	class ResourceManager
	{
	public:

		ResourceManager() = default;
		~ResourceManager() = default;

		TextureHandle getTextureHandle(const std::string& texturePath) const
		{
			// here I'll return a missing texture texture generated on the fly if the texture is not loaded
			return m_textureHandles.at(texturePath);
		}

		const sf::Texture& getTexture(TextureHandle handle) const
		{
			return m_textures.at(handle.id);
		}

		void loadTextures(const std::string& directoryPath)
		{
			if (!std::filesystem::exists(directoryPath))
			{
				return;
			}

			// maybe test this, dunno if it would work well honestly

			for (const auto& entry : std::filesystem::recursive_directory_iterator(directoryPath))
			{
				try
				{
					m_textures.emplace_back(entry.path());
					m_textureHandles.insert(std::pair{ entry.path().string(), TextureHandle(m_textures.size() - 1) });
				}
				catch (const sf::Exception& e) {}
			}
		}

	private:

		std::unordered_map<std::string, TextureHandle> m_textureHandles;
		std::vector<sf::Texture> m_textures;
	};
}