#pragma once

#include "Math/Vector2.h"

namespace meow {
	class Texture;
	class Rect;
	class Color;

	struct Renderable
	{
		enum class BlendMode
		{
			NONE,
			BLEND,
			ADD,
			MOD
		};
		Texture* texture;
		Rect* slice;
		float alphaMod;
		BlendMode blendMode;
		Color* colorMod;
	};
}
