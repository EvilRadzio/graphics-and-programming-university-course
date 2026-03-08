#include <algorithm>

#include "Platforming.hpp"

Scenes::Platforming::Platforming(px::ApiScene api) :
	Scene(api),
	m_map(api.tiles.emptyHandle(), 10, 10),
	m_input(api.input)
{
	m_input.set(Action::Jump, sf::Keyboard::Scancode::Space);
	m_input.set(Action::Left, sf::Keyboard::Scancode::A);
	m_input.set(Action::Right, sf::Keyboard::Scancode::D);

	for (size_t y = 0; y < m_map.height(); ++y) for (size_t x = 0; x < m_map.width(); ++x)
	{
		if (x == 0 || x == m_map.width() - 1 || y == 0 || y == m_map.height() - 1)
		{
			m_map.set(sf::Vector2u(x, y), sceneApi.tiles.handle("solid_block"));
		}
	}

	auto player = m_entities.spawn();
	m_entities.add<Transform>(player, sf::Vector2f(3.5f, 3.5f), sf::Vector2f(0.0f, 0.0f));
	m_entities.add<Hitbox>(player, 0.5f);
	m_entities.add<Controllable>(player);
}

void Scenes::Platforming::updateGui(Context& context, px::ApiUpdateGui& api)
{
	ImGuiViewport* viewport = ImGui::GetMainViewport();

	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);

	if (ImGui::Begin("##Menu", nullptr,
		ImGuiWindowFlags_NoDecoration |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoBackground))
	{
		if (ImGui::Button("Go back"))
		{
			popScene();
		}
	}
	ImGui::End();
}

void Scenes::Platforming::update(Context& context, px::ApiUpdate& api)
{
	playerControlSystem(api);

	for (auto [e, transform] : m_entities.view<Transform>())
	{
		transform.pos.x += transform.vel.x * api.dt.asSeconds();
	}

	colisionSystem();

	for (auto [e, transform] : m_entities.view<Transform>())
	{
		transform.pos.y += transform.vel.y * api.dt.asSeconds();
	}

	colisionSystem();

	m_entities.despawn();
}

void Scenes::Platforming::draw(const Context& context, px::ApiDraw& api) const
{
	sf::RectangleShape tileRect(static_cast<sf::Vector2f>(api.window.getSize()) / 10.0f);
	uint32_t tileSide = 720 / m_map.width();

	for (size_t y = 0; y < m_map.height(); ++y) for (size_t x = 0; x < m_map.width(); ++x)
	{
		sf::Vector2u position(x, y);
		px::TileHandle handle = m_map.at(position);
		if (api.tileTextures.hasTexture(handle))
		{
			tileRect.setPosition(static_cast<sf::Vector2f>(position * tileSide));
			tileRect.setTexture(&api.textures.texture(api.tileTextures.getTexture(handle)));

			api.window.draw(tileRect);
		}
	}

	for (auto [entity, _] : m_entities.view<Controllable>())
	{
		sf::RectangleShape playerRect(sf::Vector2f(tileSide, tileSide));
		playerRect.setOrigin(sf::Vector2f(tileSide /2, tileSide / 2));
		playerRect.setTexture(&api.textures.texture("player"));
		playerRect.setPosition(m_entities.get<Transform>(entity).pos * static_cast<float>(tileSide));

		api.window.draw(playerRect);
	}
}

void Scenes::Platforming::colisionSystem()
{
	for (auto [e, transform] : m_entities.view<Transform>())
	{
		if (!m_entities.has<Hitbox>(e)) continue;

		auto& hitbox = m_entities.get<Hitbox>(e);

		auto [minX, maxX] = std::minmax({ transform.pos.x - hitbox.r, transform.pos.x + hitbox.r });
		auto [minY, maxY] = std::minmax({ transform.pos.y - hitbox.r, transform.pos.y + hitbox.r });

		sf::Vector2f resolution(0.0f, 0.0f);

		for (uint32_t y = minY; y <= maxY; ++y) for (uint32_t x = minX; x <= maxX; ++x)
		{
			if (sceneApi.tiles.tile(m_map.at(sf::Vector2u(x, y))).type == px::TileType::air) continue;

			sf::Vector2f closest(
				std::clamp(transform.pos.x, static_cast<float>(x), x + 1.0f),
				std::clamp(transform.pos.y, static_cast<float>(y), y + 1.0f)
			);

			auto difference = transform.pos - closest;
			auto distanceSquared = difference.dot(difference);
			if (distanceSquared > hitbox.r * hitbox.r) continue;

			float distance = sqrtf(distanceSquared);
			sf::Vector2f normal = difference / distance;
			float penetration = hitbox.r - distance;

			resolution += normal * penetration;

			if (normal.y < -0.7f)
			{
				transform.vel.y = 0.0f;
			}
		}

		transform.pos += resolution;
	}
}

void Scenes::Platforming::playerControlSystem(px::ApiUpdate& api)
{
	constexpr float k_acceleration = 20.0f;
	constexpr float k_deceleration = 20.0f;
	constexpr float k_maxSpeed = 4.0f;
	constexpr float k_jumpVelocity = 16.0f;
	constexpr float k_downAcceleration = 30.0f;
	constexpr float k_maxDownAcceleration = 30.0f;

	for (auto [e, _] : m_entities.view<Controllable>())
	{
		if (m_entities.has<Transform>(e))
		{
			auto& transform = m_entities.get<Transform>(e);

			bool playerGrounded = sceneApi.tiles.tile(m_map.at(
				sf::Vector2u(transform.pos.x, transform.pos.y + m_entities.get<Hitbox>(e).r + 1e-6f)
			)).type != px::TileType::air;

			if (m_input.isPressed(Action::Jump) && playerGrounded)
			{
				transform.vel.y = -k_jumpVelocity;
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