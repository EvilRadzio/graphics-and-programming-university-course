#pragma once

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

		// This mutable is a bandaid fix, the manager needs a const view and const functions
		mutable EntityManager m_entities;

		px::SpriteInstance m_playerSprite;
		sf::Time m_elapsed;

		Map m_map;

		enum class Action : uint8_t
		{
			Jump, Left, Right
		};
		static constexpr uint8_t k_actions = static_cast<uint8_t>(Action::Right) + 1;

		px::InputMapping<Action, k_actions> m_input;
	};
}