#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <assert.h>
#include <filesystem>
#include <iostream>

#include <SFML/Graphics.hpp>

#include "TextureHandle.hpp"

namespace px
{
	class TextureManager
	{
	public:

		TextureManager()
		{
			sf::Image errorTexture(sf::Vector2u(1, 1), sf::Color::Magenta);
			m_textures.push_back(sf::Texture(errorTexture));
		}

		TextureHandle handle(const std::string& texturePath) const
		{
			if (!m_textureHandles.count(texturePath))
			{
				return TextureHandle(0);
			}
			
			return m_textureHandles.at(texturePath);
		}

		const sf::Texture& texture(const std::string& path) const
		{
			if (!m_textureHandles.count(path))
			{
				return m_textures.at(0);
			}

			return m_textures[m_textureHandles.at(path).id];
		}

		const sf::Texture& texture(TextureHandle handle) const
		{
			return m_textures[handle.id];
		}

		void loadRecursive(const std::string& directoryPath)
		{
			if (!std::filesystem::exists(directoryPath))
			{
				return;
			}

			for (const auto& entry : std::filesystem::recursive_directory_iterator(directoryPath))
			{
				if (!entry.is_regular_file())
				{
					continue;
				}

				sf::Texture texture;
				if (!texture.loadFromFile(entry.path()))
				{
					continue;
				}
				
				m_textures.push_back(std::move(texture));

				std::string path = std::filesystem::relative(entry.path(), directoryPath).replace_extension().generic_string();

				std::cout << "Loaded: " << path << std::endl;

				m_textureHandles.insert(std::pair{ 
					path,
					TextureHandle(m_textures.size() - 1)
				});
			}
		}

	private:

		std::unordered_map<std::string, TextureHandle> m_textureHandles;
		std::vector<sf::Texture> m_textures;
	};
}