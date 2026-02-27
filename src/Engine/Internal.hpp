#pragma once

#include <type_traits>
#include <concepts>

namespace Engine
{
	template <typename T>
	concept Internal = requires {
		typename T::Context;
		typename T::SceneId;
	} && std::is_enum_v<typename T::SceneId>;
}