#pragma once

#include <optional>
#include <memory>
#include "Core/Utils.h"

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

		std::shared_ptr<Texture> texture;
		std::optional<Rect> slice;
		std::optional<float> alphaMod;
		std::optional<BlendMode> blendMode;
		std::optional<Color> colorMod;
		void draw(std::optional<Rect> dst);
	};
}
