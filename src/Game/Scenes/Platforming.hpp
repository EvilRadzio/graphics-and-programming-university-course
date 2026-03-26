#pragma once

#include "Game/Types.hpp"

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

		struct Hitbox
		{
			sf::Rect<float> rect{};
		};

		struct Transform
		{
			sf::Vector2f pos{};
			sf::Vector2f vel{};
		};

		struct Gravity{};

		struct Controllable{
			bool canJump{};
		};

		// This mutable is a bandaid fix, the manager needs a const view and const functions
		mutable px::EntityManager<
			Hitbox,
			Transform,
			Gravity,
			Controllable
		> m_entities;

		px::EntitySprite m_playerSprite;
		sf::Time m_elapsed;

		px::Grid<Tile> m_map;

		enum class Action : uint8_t
		{
			Jump, Left, Right
		};
		static constexpr uint8_t k_actions = static_cast<uint8_t>(Action::Right) + 1;

		px::InputMapping<Action, k_actions> m_input;
	};
}