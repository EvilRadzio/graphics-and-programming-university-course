#pragma once

namespace Engine::Input
{
	class Raw;
}

namespace Engine::World::Tiles
{
	class Manager;
}

namespace Engine::Apis
{
	struct Scene
	{
		const Input::Raw& input;
		const World::Tiles::Manager& tiles;
	};
}