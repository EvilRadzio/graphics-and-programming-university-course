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
		virtual void setTopSceneTransparency(bool transparent) = 0;
	};

	template <Internal I>
	class SceneStack : public SceneCommands<I>
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
		void setTopSceneTransparency(bool transparent) override;

		void registerScene(I::SceneId id, SceneFactory&& factoryFunction) { m_factories[id] = std::move(factoryFunction); }
		bool empty() { return m_scenes.empty(); }

	private:

		enum class SceneAction { Push, Replace, Pop, PopUntil };

		struct SceneRequest
		{
			SceneAction action;
			std::optional<typename I::SceneId> requested;
			std::optional<typename I::ScenePayload> payload;
		};

		struct SceneInstance
		{
			std::unique_ptr<Scene<I>> ptr{};
			I::SceneId id{};
			bool isTransparent{};
		};

		void update(ApiUpdate& api);
		void draw(ApiDraw& api) const;

		std::unordered_map<typename I::SceneId, SceneFactory> m_factories;
		std::vector<SceneInstance> m_scenes;
		std::optional<SceneRequest> m_request;
		
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
	inline void SceneStack<I>::setTopSceneTransparency(bool transparent)
	{
		assert(!m_scenes.empty() && "Can't set a transparency on a top scene if there is no top scene");
		m_scenes.back().isTransparent = transparent;
	}

	template <Internal I>
	inline void SceneStack<I>::draw(ApiDraw& api) const
	{
		assert(!m_scenes.empty());

		const size_t count = m_scenes.size();
		size_t firstRenderable = count;
		for (size_t i = count - 1; i < count; --i)
		{
			firstRenderable = i;

			if (!m_scenes[i].isTransparent)
			{
				break;
			}
		}

		for (size_t i = firstRenderable; i < count; ++i)
		{
			m_scenes[i].ptr->draw(api);
		}
	}

	template <Internal I>
	inline void SceneStack<I>::update(ApiUpdate& api)
	{
		if (const SceneRequest* request = m_request.has_value() ? &m_request.value() : nullptr)
		{
			const I::SceneId* requested = request->requested.has_value() ? &request->requested.value() : nullptr;

			switch (request->action)
			{
			case SceneAction::Pop:

				assert(!m_scenes.empty() && "Can't pop a scene from an empty scene stack");
				m_scenes.pop_back();
				break;

			case SceneAction::Push:

				assert(m_factories.count(*requested) && "Can't push an unregistered scene onto a scene stack");
				m_scenes.push_back({ nullptr, *requested });
				m_scenes.back().ptr = m_factories.at(*requested)();
				break;

			case SceneAction::Replace:

				assert(!m_scenes.empty() && m_factories.count(*requested) &&
					"Can't replace a scene if there is no scene in a scene stack or a requested type is not registered");
				m_scenes.pop_back();
				m_scenes.push_back({ nullptr, *requested });
				m_scenes.back().ptr = m_factories.at(*requested)();
				break;

			case SceneAction::PopUntil:

				while (m_scenes.back().id != *requested)
				{
					assert(!m_scenes.empty() && m_factories.count(*requested) && "Can't pop until a scene that doesn't exist in a scene stack");
					m_scenes.pop_back();
				}
				break;
			}
			m_scenes.back().ptr->onEnter(request->payload.has_value() ? &request->payload.value() : nullptr);
			m_request = {};
		}

		assert(!m_scenes.empty());

		m_scenes.back().ptr->update(api);
	}
}