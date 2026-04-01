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

	namespace impl
	{
		enum class SceneAction { Push, Replace, Pop, PopUntil };

		template <Internal I>
		struct SceneRequest
		{
			SceneAction action;
			std::optional<typename I::SceneId> requested;
			std::optional<typename I::ScenePayload> payload;
		};
	}

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

		void registerScene(I::SceneId id, SceneFactory&& factoryFunction) { m_factories[id] = std::move(factoryFunction); }
		void pushScene(I::SceneId id) { assert(m_factories.count(id));  m_scenes.push_back(m_factories[id]()); m_sceneIds.push_back(id); }
		void popScene() { assert(!m_scenes.empty());  m_scenes.pop_back(); m_sceneIds.pop_back(); }
		bool empty() { return m_scenes.empty(); }

	private:

		void update(ApiUpdate& api)
		{
			if (const impl::SceneRequest<I>* request = m_request.has_value() ? &m_request.value() : nullptr)
			{
				const I::SceneId* requested = request->requested.has_value() ? &request->requested.value() : nullptr;

				switch (request->action)
				{
				case impl::SceneAction::Pop:

					assert(!m_scenes.empty() && "Can't pop a scene from an empty scene stack");
					m_scenes.pop_back();
					m_sceneIds.pop_back();
					break;

				case impl::SceneAction::Push:

					assert(m_factories.count(*requested) && "Can't push an unregistered scene onto a scene stack");
					m_scenes.push_back(m_factories.at(*requested)());
					m_sceneIds.push_back(*requested);
					break;

				case impl::SceneAction::Replace:

					assert(!m_scenes.empty() && m_factories.count(*requested) &&
						"Can't replace a scene if there is no scene in a scene stack or a requested type is not registered");
					m_scenes.pop_back();
					m_scenes.push_back(m_factories.at(*requested)());
					m_sceneIds.back() = *requested;
					break;

				case impl::SceneAction::PopUntil:

					while (m_sceneIds.back() != *requested)
					{
						assert(!m_scenes.empty() && m_factories.count(*requested) && "Can't pop until a scene that doesn't exist in a scene stack");
						m_scenes.pop_back();
						m_sceneIds.pop_back();
					}
					break;
				}
				m_scenes.back()->onEnter(request->payload.has_value() ? &request->payload.value() : nullptr);
				m_request = {};
			}

			assert(!m_scenes.empty());

			m_scenes.back()->update(api);
		}

		void draw(ApiDraw& api) const
		{
			assert(!m_scenes.empty());
			
			const size_t count = m_scenes.size();
			size_t firstRenderable = count;
			for (size_t i = count - 1; i < count; --i)
			{
				firstRenderable = i;

				if (!m_scenes[i]->getProperties().renderThrough)
				{
					break;
				}
			}

			for (size_t i = firstRenderable; i < count; ++i)
			{
				m_scenes[i]->draw(api);
			}
		}

		std::unordered_map<typename I::SceneId, SceneFactory> m_factories;
		std::vector<std::unique_ptr<Scene<I>>> m_scenes;
		std::vector<typename I::SceneId> m_sceneIds;
		std::optional<impl::SceneRequest<I>> m_request;
		// ids really shouldn't be in a separate vector
		
		friend Client<I>;
	};

	template <Internal I>
	inline void SceneStack<I>::push(I::SceneId id)
	{
		m_request = { impl::SceneAction::Push, id, {} };
	}

	template <Internal I>
	inline void SceneStack<I>::push(I::SceneId id, I::ScenePayload payload)
	{
		m_request = { impl::SceneAction::Push, id, payload };
	}

	template <Internal I>
	inline void SceneStack<I>::pop()
	{
		m_request = { impl::SceneAction::Pop, {}, {}};
	}

	template <Internal I>
	inline void SceneStack<I>::pop(I::ScenePayload payload)
	{
		m_request = { impl::SceneAction::Pop, {}, payload };
	}

	template <Internal I>
	inline void SceneStack<I>::replace(I::SceneId id)
	{
		m_request = { impl::SceneAction::Replace, id, {} };
	}

	template <Internal I>
	inline void SceneStack<I>::replace(I::SceneId id, I::ScenePayload payload)
	{
		m_request = { impl::SceneAction::Replace, id, payload };
	}

	template <Internal I>
	inline void SceneStack<I>::popUntil(I::SceneId id)
	{
		m_request = { impl::SceneAction::PopUntil, id, {} };
	}

	template <Internal I>
	inline void SceneStack<I>::popUntil(I::SceneId id, I::ScenePayload payload)
	{
		m_request = { impl::SceneAction::PopUntil, id, payload };
	}
}