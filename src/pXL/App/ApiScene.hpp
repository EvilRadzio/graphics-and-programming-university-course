#pragma once

namespace px
{
	class InputRaw;
	class Assets;
	class SceneCommands;
	class SceneConfig;
	class Transition;

	struct ApiScene
	{
		SceneCommands& comms;
		const InputRaw& input;
		const Assets& assets;
		SceneConfig& properties;
		Transition& transition;
	};
}