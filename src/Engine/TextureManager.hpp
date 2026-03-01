#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <assert.h>
#include <filesystem>
#include <iostream>

#include <SFML/Graphics.hpp>

#include "TextureHandle.hpp"

namespace Engine
{
	class TextureManager
	{
	public:

		TextureManager()
		{
			sf::Image errorTexture(sf::Vector2u(1, 1), sf::Color::Magenta);
			m_textures.push_back(sf::Texture(errorTexture));
		}

		TextureHandle getHandle(const std::string& texturePath) const
		{
			if (!m_textureHandles.count(texturePath))
			{
				return TextureHandle(0);
			}
			
			return m_textureHandles.at(texturePath);
		}

		const sf::Texture& getTexture(TextureHandle handle) const
		{
			return m_textures.at(handle.id);
		}

		void loadAll(const std::string& directoryPath)
		{
			if (!std::filesystem::exists(directoryPath))
			{
				return;
			}

			// maybe test this, dunno if it would work well honestly

			for (const auto& entry : std::filesystem::recursive_directory_iterator(directoryPath))
			{
				if (!entry.is_regular_file())
				{
					continue;
				}

				try
				{
					m_textures.emplace_back(entry.path());
					m_textureHandles.insert(std::pair{ 
						std::filesystem::relative(entry.path(), directoryPath).replace_extension().string(),
						TextureHandle(m_textures.size() - 1)
					});
				}
				catch (const sf::Exception& e) {}
			}
		}

	private:

		std::unordered_map<std::string, TextureHandle> m_textureHandles;
		std::vector<sf::Texture> m_textures;
	};
}