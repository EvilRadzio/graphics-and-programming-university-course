#pragma once

namespace sf
{
	class RenderTarget;
}

namespace px
{
	class Assets;

	struct ApiDraw
	{
		sf::RenderTarget& window;
		const Assets& assets;
	};
}