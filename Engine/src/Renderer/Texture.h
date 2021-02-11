#pragma once

#include <string>
#include "Math/Vector2.h"

namespace meow {
	class Texture
	{
	public:
		void* getRawData();
	};

	class Image :public Texture
	{
	public:
		Image(std::string_view filename);
		void setColorToAlpha(int r, int g, int b);

	};

	class Canvas :public Texture
	{
	public:
		Canvas(int width, int height);
		void pushLayer(Texture* tex, Vector2i pos);

	};
}
