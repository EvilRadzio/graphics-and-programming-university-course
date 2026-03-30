#include <algorithm>

#include "Platforming.hpp"

Scenes::Platforming::Platforming(ApiScene& api, Context& ctx) :
	Scene(api, ctx),
	m_playerSprite{ "knight", "", {} },
	m_map(sf::Vector2u(10, 10), ctx.tiles["empty"]),
	m_input(api.input)
{
	m_input.set(Action::Jump, sf::Keyboard::Scancode::Space);
	m_input.set(Action::Left, sf::Keyboard::Scancode::A);
	m_input.set(Action::Right, sf::Keyboard::Scancode::D);

	for (uint32_t y = 0; y < m_map.size().y; ++y)
	{
		for (uint32_t x = 0; x < m_map.size().x; ++x)
		{
			if (x == 0 || x == m_map.size().x - 1 || y == 0 || y == m_map.size().y - 1)
			{
				m_map.at({ x, y }) = ctx.tiles["solid_block"];
			}
		}
	}

	m_map.at({ 7, 7 }) = ctx.tiles.at("solid_block");
	m_map.at({ 4, 3 }) = ctx.tiles.at("solid_block");
	m_map.at({ 6, 7 }) = ctx.tiles.at("solid_block");
	m_map.at({ 2, 5 }) = ctx.tiles.at("solid_block");
	m_map.at({ 5, 4 }) = ctx.tiles.at("solid_block");

	auto player = m_entities.spawn();
	m_entities.add<Transform>(player, sf::Vector2f(3.5f, 3.5f), sf::Vector2f(0.0f, 0.0f));
	m_entities.add<Hitbox>(player,
		sf::Rect<float>(
			sf::Vector2f(-0.25f, -0.25f),
			sf::Vector2f(0.5f, 0.75f)
		)
	);
	m_entities.add<Controllable>(player);
}

void Scenes::Platforming::update(px::ApiUpdate& api)
{
	if (scene.input.isPressed(sf::Keyboard::Scancode::Escape))
	{
		scene.comms.push(SceneId::Pause, {});
	}

	m_elapsed += api.dt;

	playerControlSystem(api);

	movementAndColisionSystem(api);

	m_entities.despawn();
}

void Scenes::Platforming::draw(px::ApiDraw& api) const
{
	api.window.clear(sf::Color::Blue);

	for (auto [e, _] : m_entities.view<Controllable>())
	{
		const auto& position = m_entities.get<Transform>(e).pos;

		sf::View view(
			position * static_cast<float>(api.window.getSize().x / 10.0f),
			static_cast<sf::Vector2f>(api.window.getSize())
		);

		api.window.setView(view);
	}

	sf::Vector2u size = m_map.size();
	uint32_t tileSide = 720 / size.x;

	for (size_t y = 0; y < size.y; ++y) for (size_t x = 0; x < size.x; ++x)
	{
		sf::Vector2u position(x, y);

		if (m_map.at(position).sprite != "")
		{
			auto sprite(api.assets.tileSprites.get(m_map.at(position).sprite).get(getAdjacent(m_map, position), api.assets.textures));

			sprite.setPosition(sf::Vector2f{
				static_cast<float>(x * tileSide),
				static_cast<float>(y * tileSide)
			});

			auto bounds = sprite.getLocalBounds();
			sprite.setScale(sf::Vector2f{
				static_cast<float>(tileSide) / bounds.size.x,
				static_cast<float>(tileSide) / bounds.size.y
			});

			api.window.draw(sprite);
		}
	}

	for (auto [entity, _] : m_entities.view<Controllable>())
	{
		auto& transform = m_entities.get<Transform>(entity);
		if (transform.vel.x == 0.0f)
		{
			sf::Sprite sprite(api.assets.textures.get("knight"));
			sprite.setTextureRect(api.assets.entitySprites.get("knight").clips.at("idle").getFrameRect(m_elapsed));
			auto bounds = sprite.getLocalBounds();
			auto spriteBounds = api.assets.entitySprites.get("knight").rect;
			sprite.setPosition(transform.pos * static_cast<float>(tileSide) + static_cast<sf::Vector2f>(spriteBounds.position));
			sprite.setScale({ spriteBounds.size.x / bounds.size.x, spriteBounds.size.y / bounds.size.y });

			api.window.draw(sprite);
		}
		else
		{
			sf::Sprite sprite(api.assets.textures.get("knight"));
			sprite.setTextureRect(api.assets.entitySprites.get("knight").clips.at("run").getFrameRect(m_elapsed));
			auto bounds = sprite.getLocalBounds();
			auto spriteBounds = api.assets.entitySprites.get("knight").rect;
			sprite.setPosition(transform.pos * static_cast<float>(tileSide) + static_cast<sf::Vector2f>(spriteBounds.position));
			sprite.setScale({ spriteBounds.size.x / bounds.size.x, spriteBounds.size.y / bounds.size.y });

			api.window.draw(sprite);
		}
	}

	api.window.setView(api.window.getDefaultView());
}

void Scenes::Platforming::playerControlSystem(px::ApiUpdate& api)
{
	constexpr float k_acceleration = 25.0f;
	constexpr float k_deceleration = 25.0f;
	constexpr float k_maxSpeed = 5.0f;
	constexpr float k_jumpVelocity = 13.25f;
	constexpr float k_downAcceleration = 30.0f;
	constexpr float k_maxDownAcceleration = 20.0f;

	for (auto [e, controllable] : m_entities.view<Controllable>())
	{
		if (m_entities.has<Transform>(e))
		{
			auto& transform = m_entities.get<Transform>(e);

			if (m_input.isPressed(Action::Jump) && controllable.canJump)
			{
				transform.vel.y = -k_jumpVelocity;
				controllable.canJump = false;
			}
			else
			{
				transform.vel.y = std::min(transform.vel.y + k_downAcceleration * api.dt.asSeconds(), k_maxDownAcceleration);
			}

			int32_t direction = 0 - m_input.isHeld(Action::Left) + m_input.isHeld(Action::Right);

			transform.vel.x += (direction * k_acceleration * api.dt.asSeconds());

			if (!direction)
			{
				float newVelocity = std::abs(transform.vel.x) - k_deceleration * api.dt.asSeconds();

				if (newVelocity < 0.0f)
				{
					transform.vel.x = 0.0f;
					continue;
				}

				transform.vel.x = (transform.vel.x > 0.0f ? 1.0f : -1.0f) * newVelocity;
				continue;
			}

			if (std::abs(transform.vel.x) > k_maxSpeed)
			{
				transform.vel.x = (transform.vel.x > 0.0f ? 1.0f : -1.0f) * k_maxSpeed;
			}
		}
	}
}

void Scenes::Platforming::movementAndColisionSystem(px::ApiUpdate& api)
{
	// The grounded check is stupid but what can you co? will fix it later 
	for (auto [e, transform] : m_entities.view<Transform>())
	{
		if (!m_entities.has<Hitbox>(e))
		{
			transform.pos += transform.vel * api.dt.asSeconds();
			continue;
		}

		auto rect = m_entities.get<Hitbox>(e).rect;

		int32_t minY = rect.position.y + transform.pos.y;
		int32_t maxY = rect.position.y + rect.size.y + transform.pos.y;

		if (transform.vel.x < 0.0f)
		{
			float currentX = rect.position.x + transform.pos.x;
			float possibleX = currentX + transform.vel.x * api.dt.asSeconds();

			bool colided = false;
			while (currentX - 1e-3f > possibleX && !colided)
			{
				for (size_t y = minY; y <= maxY; ++y)
				{
					if (m_map.at(sf::Vector2u(currentX - 1e-3f, y)).type != Tile::Type::Air)
					{
						colided = true;
						break;
					}
				}

				if (!colided)
				{
					currentX -= 1e-3f;
				}
			}

			transform.pos.x = currentX - rect.position.x;
		}
		else if (transform.vel.x > 0.0f)
		{
			float currentX = rect.position.x + rect.size.x + transform.pos.x;
			float possibleX = currentX + transform.vel.x * api.dt.asSeconds();

			bool colided = false;
			while (currentX + 1e-3f < possibleX && !colided)
			{
				for (size_t y = minY; y <= maxY; ++y)
				{
					if (m_map.at(sf::Vector2u(currentX + 1e-3f, y)).type != Tile::Type::Air)
					{
						colided = true;
						break;
					}
				}

				if (!colided)
				{
					currentX += 1e-3f;
				}
			}

			transform.pos.x = currentX - (rect.size.x + rect.position.x);
		}

		int32_t minX = rect.position.x + transform.pos.x;
		int32_t maxX = rect.position.x + rect.size.x + transform.pos.x;

		if (m_entities.has<Controllable>(e))
		{
			m_entities.get<Controllable>(e).canJump = false;
		}

		if (transform.vel.y < 0.0f)
		{
			float currentY = rect.position.y + transform.pos.y;
			float possibleY = currentY + transform.vel.y * api.dt.asSeconds();

			bool colided = false;
			while (currentY - 1e-3f > possibleY && !colided)
			{
				for (size_t x = minX; x <= maxX; ++x)
				{
					if (m_map.at(sf::Vector2u(x, currentY - 1e-3f)).type != Tile::Type::Air)
					{
						colided = true;
						break;
					}
				}

				if (!colided)
				{
					currentY -= 1e-3f;
				}
				else
				{
					transform.vel.y = 0.0f;
				}
			}

			transform.pos.y = currentY - rect.position.y;
		}
		else if (transform.vel.y > 0.0f)
		{
			float currentY = rect.position.y + rect.size.y + transform.pos.y;
			float possibleY = currentY + transform.vel.y * api.dt.asSeconds();

			bool colided = false;
			while (currentY + 1e-3f < possibleY && !colided)
			{
				for (size_t x = minX; x <= maxX; ++x)
				{
					if (m_map.at(sf::Vector2u(x, currentY + 1e-3f)).type != Tile::Type::Air)
					{
						colided = true;
						break;
					}
				}

				if (!colided)
				{
					currentY += 1e-3f;
				}
				else
				{
					transform.vel.y = 0.0f;
					if (m_entities.has<Controllable>(e))
					{
						m_entities.get<Controllable>(e).canJump = true;
					}
				}
			}

			transform.pos.y = currentY - (rect.size.y + rect.position.y);
		}
	}
}