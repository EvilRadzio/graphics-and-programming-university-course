#pragma once

#include <vector>
#include <memory>
#include <functional>
#include <unordered_map>
#include <assert.h>

#include "Scene.hpp"

namespace px
{
	template <Internal I>
	class Client;

	template <Internal I>
	class SceneCommands
	{
	public:

		virtual void push(I::SceneId id) = 0;
		virtual void push(I::SceneId id, I::ScenePayload payload) = 0;
		virtual void pop() = 0;
		virtual void pop(I::ScenePayload payload) = 0;
		virtual void replace(I::SceneId id) = 0;
		virtual void replace(I::SceneId id, I::ScenePayload payload) = 0;
		virtual void popUntil(I::SceneId id) = 0;
		virtual void popUntil(I::SceneId id, I::ScenePayload payload) = 0;
		virtual bool isRequestPending() const = 0;
	};

	class SceneConfig
	{
	public:

		virtual void setTransparency(bool transparent) = 0;
	};

	template <Internal I>
	class SceneStack final : public SceneCommands<I>, public SceneConfig
	{
	public:

		using SceneFactory = std::function<std::unique_ptr<Scene<I>>()>;

		void push(I::SceneId id) override;
		void push(I::SceneId id, I::ScenePayload payload) override;
		void pop() override;
		void pop(I::ScenePayload payload) override;
		void replace(I::SceneId id) override;
		void replace(I::SceneId id, I::ScenePayload payload) override;
		void popUntil(I::SceneId id) override;
		void popUntil(I::SceneId id, I::ScenePayload payload) override;
		bool isRequestPending() const override;

		void registerScene(I::SceneId id, SceneFactory factoryFunction);
		bool empty() const;

	private:

		void setTransparency(bool transparent) override;

		enum class SceneAction : uint8_t { Push, Replace, Pop, PopUntil };

		struct SceneRequest
		{
			SceneAction action;
			std::optional<typename I::SceneId> requested;
			std::optional<typename I::ScenePayload> payload;
		};

		struct SceneInstance
		{
			SceneInstance(I::SceneId id) : id(id) {}

			std::unique_ptr<Scene<I>> ptr{};
			I::SceneId id;
			bool isTransparent{};
		};

		void pushScene(I::SceneId id);
		void popScene();

		void flush();
		void update(ApiUpdate& api);
		void draw(ApiDraw& api) const;

		std::unordered_map<typename I::SceneId, SceneFactory> m_factories;
		std::vector<SceneInstance> m_scenes;
		std::optional<SceneRequest> m_request;
		Transition m_transition;
		
		friend Client<I>;
	};

	template <Internal I>
	inline void SceneStack<I>::push(I::SceneId id)
	{
		m_request = { SceneAction::Push, id, {} };
	}

	template <Internal I>
	inline void SceneStack<I>::push(I::SceneId id, I::ScenePayload payload)
	{
		m_request = { SceneAction::Push, id, payload };
	}

	template <Internal I>
	inline void SceneStack<I>::pop()
	{
		m_request = { SceneAction::Pop, {}, {}};
	}

	template <Internal I>
	inline void SceneStack<I>::pop(I::ScenePayload payload)
	{
		m_request = { SceneAction::Pop, {}, payload };
	}

	template <Internal I>
	inline void SceneStack<I>::replace(I::SceneId id)
	{
		m_request = { SceneAction::Replace, id, {} };
	}

	template <Internal I>
	inline void SceneStack<I>::replace(I::SceneId id, I::ScenePayload payload)
	{
		m_request = { SceneAction::Replace, id, payload };
	}

	template <Internal I>
	inline void SceneStack<I>::popUntil(I::SceneId id)
	{
		m_request = { SceneAction::PopUntil, id, {} };
	}

	template <Internal I>
	inline void SceneStack<I>::popUntil(I::SceneId id, I::ScenePayload payload)
	{
		m_request = { SceneAction::PopUntil, id, payload };
	}

	template <Internal I>
	inline bool SceneStack<I>::isRequestPending() const
	{
		return m_request.has_value();
	}

	template <Internal I>
	inline void SceneStack<I>::registerScene(I::SceneId id, SceneFactory factoryFunction)
	{
		m_factories.insert_or_assign(id, std::move(factoryFunction));
	}

	template <Internal I>
	inline bool SceneStack<I>::empty() const
	{
		return m_scenes.empty();
	}

	template <Internal I>
	inline void SceneStack<I>::setTransparency(bool transparent)
	{
		assert(!m_scenes.empty() && "Can't set a transparency on a top scene if there is no top scene");
		m_scenes.back().isTransparent = transparent;
	}

	template <Internal I>
	inline void SceneStack<I>::pushScene(I::SceneId id)
	{
		assert(m_factories.count(id) && "Can't push() an unregistered scene onto a scene stack");
		m_scenes.emplace_back(id);
		m_scenes.back().ptr = m_factories.at(id)();
	}

	template <Internal I>
	inline void SceneStack<I>::popScene()
	{
		assert(!m_scenes.empty() && "Can't pop() a scene from an empty scene stack");
		m_scenes.pop_back();
	}

	template <Internal I>
	inline void SceneStack<I>::flush()
	{
		if (!m_request)
		{
			return;
		}

		const SceneRequest request = m_request.value();
		m_request = {};
		const I::SceneId* requested = request.requested ? &request.requested.value() : nullptr;

		if (request.action == SceneAction::Pop)
		{
			popScene();
		}
		else if (request.action == SceneAction::PopUntil)
		{
			assert(std::any_of(m_scenes.begin(), m_scenes.end(), [&](const SceneInstance& scene) { return scene.id == *requested; }),
				"Can't popUntil() to a scene that's not on the stack");

			while (m_scenes.back().id != *requested)
			{
				popScene();
			}
		}
		else
		{
			if (request.action == SceneAction::Replace)
			{
				popScene();
			}
			pushScene(*requested);
		}

		assert(!m_scenes.empty() && "Can't run onEnter() if the scene stack is empty");
		m_scenes.back().ptr->onEnter(request.payload ? &request.payload.value() : nullptr);
	}

	template <Internal I>
	inline void SceneStack<I>::update(ApiUpdate& api)
	{
		assert(!m_scenes.empty() && "Can't run update() a scene if the scene stack is empty");

		m_scenes.back().ptr->update(api);
	}

	template <Internal I>
	inline void SceneStack<I>::draw(ApiDraw& api) const
	{
		assert(!m_scenes.empty() && "Can't run draw() a scene if a scene stack is empty");

		const size_t count = m_scenes.size();
		size_t firstRenderable = count - 1;
		while (firstRenderable > 0 && m_scenes[firstRenderable].isTransparent)
		{
			--firstRenderable;
		}

		for (size_t i = firstRenderable; i < count; ++i)
		{
			m_scenes[i].ptr->draw(api);
		}
	}
}