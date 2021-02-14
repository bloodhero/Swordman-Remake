#pragma once

#include "Math/Vector2.h"

namespace meow {
	class Texture;

	struct Rect
	{
		int x;
		int y;
		int w;
		int h;
	};

	struct Color
	{
		int r;
		int g;
		int b;
		int a;
	};


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
		Rect slice;
		float alphaMod;
		BlendMode blendMode;
		Color colorMod;
	};
}
