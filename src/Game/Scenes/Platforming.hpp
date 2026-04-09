#pragma once

#include <entt/entt.hpp>

#include "Game/Types.hpp"
#include "Game/Map.hpp"

namespace Scenes
{
	class Platforming : public Scene
	{
	public:

		Platforming(ApiScene& api, Context& ctx);

		void update(px::ApiUpdate& api) override;
		void draw(px::ApiDraw& api) const override;

	private:

		void playerControlSystem(px::ApiUpdate& api);
		void movementAndColisionSystem(px::ApiUpdate& api);

		entt::registry m_registry;

		sf::Time m_elapsed;

		Map m_map;

		int32_t m_dir{ 1 };

		enum class Action : uint8_t
		{
			Jump, Left, Right
		};
		static constexpr uint8_t k_actions = static_cast<uint8_t>(Action::Right) + 1;

		px::InputMapping<Action, k_actions> m_input;
	};
}