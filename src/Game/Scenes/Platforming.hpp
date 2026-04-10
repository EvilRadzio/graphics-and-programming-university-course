#pragma once

#include <entt/entt.hpp>

#include "pXL/pXL.hpp"

#include "Game/Context.hpp"
#include "Game/Tile.hpp"
#include "Game/Map.hpp"
#include "Game/Components.hpp"

namespace Scenes
{
	class Platforming : public px::Scene
	{
	public:

		Platforming(px::ApiScene& api, Context& ctx);

		void update(px::ApiUpdate& api) override;
		void draw(px::ApiDraw& api) const override;

	private:

		void playerControlSystem(px::ApiUpdate& api);
		void movementAndColisionSystem(px::ApiUpdate& api);

		Context& m_ctx;

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