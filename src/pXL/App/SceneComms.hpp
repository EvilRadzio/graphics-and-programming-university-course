#pragma once

#include <optional>
#include <utility>

#include "Internal.hpp"

namespace px
{
	enum class SceneAction
	{
		Pop, Push, Replace, 
	};

	template <Internal I>
	class SceneStack;

	template <Internal I>
	class SceneComms
	{
	public:

		void push(const typename I::SceneId sceneId, typename I::ScenePayload&& scenePayload)
		{
			m_push.emplace(sceneId, scenePayload);
		}

		void replace(const typename I::SceneId sceneId, typename I::ScenePayload&& scenePayload)
		{
			m_replace.emplace(sceneId, scenePayload);
		}

		void pop(typename I::ScenePayload&& scenePayload)
		{
			m_pop.emplace(scenePayload);
		}
		
	private:

		std::optional<std::pair<typename I::SceneId, typename I::ScenePayload>> m_push;
		std::optional<std::pair<typename I::SceneId, typename I::ScenePayload>> m_replace;
		std::optional<typename I::ScenePayload> m_pop;

		friend SceneStack;
	};
}