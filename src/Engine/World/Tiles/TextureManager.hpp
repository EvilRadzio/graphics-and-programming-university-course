#pragma once

#include <unordered_map>
#include <string>

#include "Handle.hpp"
#include "Resources/Textures/Textures.hpp"

namespace Engine::Tiles
{
	class TextureManager
	{
	public:

		TextureManager(const Engine::Resources::Textures::Manager& textureManager)
			: m_textureManager(textureManager) {}

		~TextureManager() = default;

		void setTileTexture(const Handle tileHandle, const std::string& texturePath)
		{
			m_tileTextures.insert(std::pair{
				tileHandle.id,
				m_textureManager.getHandle(texturePath)
			});
		}

		Engine::Resources::Textures::Handle getTexture(const Handle tileHandle) const
		{
			return m_tileTextures.at(tileHandle.id);
		}

	private:

		std::unordered_map<size_t, Engine::Resources::Textures::Handle> m_tileTextures;
		const Engine::Resources::Textures::Manager& m_textureManager;
	};
}