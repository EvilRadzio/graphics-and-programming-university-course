#pragma once

#include <type_traits>
#include <concepts>

namespace px
{
	template <typename T>
	concept Internal = requires {
		typename T::Context;
		typename T::SceneId;
		typename T::ScenePayload;
	} && std::is_enum_v<typename T::SceneId>;
}