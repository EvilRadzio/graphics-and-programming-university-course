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

	m_map.set(sf::Vector2u(7, 7), sceneApi.tiles.handle("solid_block"));
	m_map.set(sf::Vector2u(4, 3), sceneApi.tiles.handle("solid_block"));
	m_map.set(sf::Vector2u(6, 7), sceneApi.tiles.handle("solid_block"));
	m_map.set(sf::Vector2u(2, 5), sceneApi.tiles.handle("solid_block"));
	m_map.set(sf::Vector2u(5, 4), sceneApi.tiles.handle("solid_block"));

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

	// Partial movement also needs to be done in some cases
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
					if (sceneApi.tiles.tile(m_map.at(sf::Vector2u(currentX - 1e-3f, y))).type != px::TileType::air)
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
					if (sceneApi.tiles.tile(m_map.at(sf::Vector2u(currentX + 1e-3f, y))).type != px::TileType::air)
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
					if (sceneApi.tiles.tile(m_map.at(sf::Vector2u(x, currentY - 1e-3f))).type != px::TileType::air)
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
				{;
					if (sceneApi.tiles.tile(m_map.at(sf::Vector2u(x, currentY + 1e-3f))).type != px::TileType::air)
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