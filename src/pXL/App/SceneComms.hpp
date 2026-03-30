#pragma once

#include <optional>
#include <utility>
#include <cassert>

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

		// make this into an interface and push the actual logic into a class so the scene stack doesn't poke into this

		enum class Action
		{
			None,
			Push,
			Replace,
			Pop,
			PopUntil
		};

		void push(const typename I::SceneId sceneId, std::optional<typename I::ScenePayload>&& scenePayload)
		{
			m_action = Action::Push;
			m_requested = sceneId;
			m_payload = std::move(scenePayload);
		}

		void replace(const typename I::SceneId sceneId, std::optional<typename I::ScenePayload>&& scenePayload)
		{
			m_action = Action::Replace;
			m_requested = sceneId;
			m_payload = std::move(scenePayload);
		}

		void pop(std::optional<typename I::ScenePayload>&& scenePayload)
		{
			m_action = Action::Pop;
			m_requested = {};
			m_payload = std::move(scenePayload);
		}

		void popUntil(const typename I::SceneId sceneId, std::optional<typename I::ScenePayload>&& scenePayload)
		{
			m_action = Action::PopUntil;
			m_requested = sceneId;
			m_payload = std::move(scenePayload);
		}

		const I::ScenePayload* getLastPayload() const { return m_payload? &m_payload.value() : nullptr; }
		
	private:

		std::optional<typename I::ScenePayload> m_payload{};
		std::optional<typename I::SceneId> m_requested{};
		Action m_action{};

		friend SceneStack<I>;
	};
}