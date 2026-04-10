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

	struct ApiDraw
	{
		sf::RenderTarget& window;
		const Assets& assets;
	};

	struct ApiUpdate
	{
		const sf::Window& window;
		const sf::Time dt;
		Transition& transition;
	};

	struct ApiScene
	{
		SceneCommands& comms;
		const InputRaw& input;
		const Assets& assets;
		SceneConfig& properties;
		Transition& transition;
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

	private:

		friend SceneStack;
	};
}