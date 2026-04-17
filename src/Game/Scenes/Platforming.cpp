#include <algorithm>

#include "Platforming.hpp"

Scenes::Platforming::Platforming(px::SceneInitCtx& ctx, Context& gctx) :
	Scene(ctx),
	m_ctx(gctx),
	m_map(sf::Vector2u(100, 10), m_ctx.tiles["empty"])
{
	for (uint32_t y = 0; y < m_map.size().y; ++y)
	{
		for (uint32_t x = 0; x < m_map.size().x; ++x)
		{
			if (x == 0 || x == m_map.size().x - 1 || y == 0 || y == m_map.size().y - 1)
			{
				m_map.at({ x, y }) = m_ctx.tiles["solid_block"];
			}
		}
	}

	m_map.at({ 7, 7 }) = m_ctx.tiles.at("solid_block");
	m_map.at({ 4, 3 }) = m_ctx.tiles.at("solid_block");
	m_map.at({ 6, 7 }) = m_ctx.tiles.at("solid_block");
	m_map.at({ 2, 5 }) = m_ctx.tiles.at("solid_block");
	m_map.at({ 5, 4 }) = m_ctx.tiles.at("solid_block");
	m_map.at({ 9, 8 }) = m_ctx.tiles.at("solid_block");

	auto player = m_registry.create();
	m_registry.emplace<Transform>(player, sf::Vector2f(3.5f, 3.5f), sf::Vector2f(0.0f, 0.0f));
	m_registry.emplace<Hitbox>(player, sf::Rect<float>(
		sf::Vector2f(-0.25f, -0.25f),
		sf::Vector2f(0.5f, 0.75f)
	));
	m_registry.emplace<Controllable>(player);
}

void Scenes::Platforming::update(px::UpdateCtx& ctx)
{
	if (api.mapping.isPressed("Pause"))
	{
		api.comms.push("Pause");
	}
}

void Scenes::Platforming::fixedUpdate(px::UpdateCtx& ctx)
{
	m_elapsed += ctx.dt;

	playerControlSystem(ctx);

	movementAndColisionSystem(ctx);
}

void Scenes::Platforming::draw(px::DrawCtx& ctx) const
{
	ctx.window.clear(sf::Color::Blue);

	sf::Vector2u size = m_map.size();
	uint32_t tileSide = 720 / size.y;

	auto view = m_registry.view<const Controllable, const Transform>();
	
	view.each([&](const auto& _, const auto& transform) {
		sf::Vector2f position = px::lerp(transform.oldPos, transform.pos, ctx.alpha);

		ctx.window.draw(px::Background(api.assets.backgrounds.get("background"), position.x * tileSide));

		sf::View view(
			position * static_cast<float>(ctx.window.getSize().x / 10.0f),
			static_cast<sf::Vector2f>(ctx.window.getSize())
		);

		ctx.window.setView(view);
	});

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

			ctx.window.draw(sprite);
		}
	}

	view.each([&](const auto& _, const auto& transform) {
		sf::Vector2f position = px::lerp(transform.oldPos, transform.pos, ctx.alpha) * static_cast<float>(tileSide);
		if (transform.vel.x == 0.0f)
		{
			px::Sprite sprite(api.assets.sprites.get("knight"), "idle", m_elapsed);
			sprite.setScale({ 4,4 });
			sprite.setOrigin({ 16, 23 });
			sprite.setPosition(position);

			if (m_dir < 0) sprite.setMirrored(true);

			ctx.window.draw(sprite);
		}
		else
		{
			px::Sprite sprite(api.assets.sprites.get("knight"), "run", m_elapsed);
			sprite.setScale({ 4,4 });
			sprite.setOrigin({ 16, 23 });
			sprite.setPosition(position);

			if (m_dir < 0) sprite.setMirrored(true);

			ctx.window.draw(sprite);
		}
	});

	ctx.window.setView(ctx.window.getDefaultView());
}

void Scenes::Platforming::playerControlSystem(px::UpdateCtx& ctx)
{
	constexpr float k_acceleration = 25.0f;
	constexpr float k_deceleration = 25.0f;
	constexpr float k_maxSpeed = 5.0f;
	constexpr float k_jumpVelocity = 13.25f;
	constexpr float k_downAcceleration = 30.0f;
	constexpr float k_maxDownAcceleration = 20.0f;

	auto view = m_registry.view<Controllable, Transform>();

	view.each([&](auto& controllable, auto& transform) {
		if (api.mapping.isPressed("Jump") && controllable.canJump)
		{
			transform.vel.y = -k_jumpVelocity;
			controllable.canJump = false;
		}
		else
		{
			transform.vel.y = std::min(transform.vel.y + k_downAcceleration * ctx.dt.asSeconds(), k_maxDownAcceleration);
		}

		int32_t direction = 0 - api.mapping.isHeld("Left") + api.mapping.isHeld("Right");

		m_dir = direction != 0 ? direction : m_dir;

		transform.vel.x += (direction * k_acceleration * ctx.dt.asSeconds());

		if (!direction)
		{
			float newVelocity = std::abs(transform.vel.x) - k_deceleration * ctx.dt.asSeconds();

			if (newVelocity < 0.0f)
			{
				transform.vel.x = 0.0f;
				return;
			}

			transform.vel.x = (transform.vel.x > 0.0f ? 1.0f : -1.0f) * newVelocity;
			return;
		}

		if (std::abs(transform.vel.x) > k_maxSpeed)
		{
			transform.vel.x = (transform.vel.x > 0.0f ? 1.0f : -1.0f) * k_maxSpeed;
		}
	});
}

void Scenes::Platforming::movementAndColisionSystem(px::UpdateCtx& ctx)
{
	// The grounded check is stupid but what can you do? will fix it later

	auto view = m_registry.view<Transform, Hitbox, Controllable>();

	view.each([&](auto& transform, auto& hitbox, auto& controllable) {
		transform.oldPos = transform.pos;

		sf::FloatRect rect = hitbox.rect;

		int32_t minY = rect.position.y + transform.pos.y;
		int32_t maxY = rect.position.y + rect.size.y + transform.pos.y;

		if (transform.vel.x < 0.0f)
		{
			float currentX = rect.position.x + transform.pos.x;
			float possibleX = currentX + transform.vel.x * ctx.dt.asSeconds();

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
			float possibleX = currentX + transform.vel.x * ctx.dt.asSeconds();

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

		controllable.canJump = false;

		if (transform.vel.y < 0.0f)
		{
			float currentY = rect.position.y + transform.pos.y;
			float possibleY = currentY + transform.vel.y * ctx.dt.asSeconds();

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
			float possibleY = currentY + transform.vel.y * ctx.dt.asSeconds();

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
					controllable.canJump = true;
				}
			}

			transform.pos.y = currentY - (rect.size.y + rect.position.y);
		}
	});
}