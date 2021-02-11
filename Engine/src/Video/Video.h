#pragma once

#include <string>
#include "Renderer/Drawable.h"

namespace meow {
	class Video :public Drawable
	{
	public:
		Video(std::string_view filename);
		void onDraw() override;
		void onUpdate(float time) override;
		void skip();
	};
}
