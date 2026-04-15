#pragma once

#include <optional>
#include <any>

namespace sf
{
	class RenderTarget;
	class Window;
}

namespace px
{
	class InputRaw;
	class Assets;
	class SceneCommands;
	class SceneConfig;
	class Transition;
	class Mapping;

	class Scene;

	struct EngineApi
	{
		SceneCommands& comms;
		const InputRaw& input;
		const Assets& assets;
		Transition& transition;
		const Mapping& mapping;
	};

	struct SceneCtx
	{
		SceneConfig& properties;

	private:

		EngineApi api;

		friend Scene;
	};

	struct UpdateCtx
	{
		const sf::Window& window;
		const sf::Time dt;
		Transition& transition;
	};

	struct DrawCtx
	{
		sf::RenderTarget& window;
		const Assets& assets;
		float alpha{};
	};

	class SceneStack;

	class Scene
	{
	public:

		Scene(const ApiScene& api) : scene(api) {}
		virtual ~Scene() = default;

		virtual void onEnter(std::any&& payload) {}
		virtual void update(ApiUpdate& api) {}
		virtual void fixedUpdate(ApiUpdate& api) {}
		virtual void draw(ApiDraw& api) const = 0;

	protected:

		ApiScene scene;
		EngineApi api;

	private:

		friend SceneStack;
	};
}