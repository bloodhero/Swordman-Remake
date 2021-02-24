#pragma once
#include "Renderer/Renderable.h"
#include "Renderer/Texture.h"
#include <optional>

namespace meow {
	class PostEffect
	{
	public:
		PostEffect();
		~PostEffect();
		void setSlice(std::optional<Rect> s);
		void setAlphaMod(std::optional<float> a);
		void setColorMod(std::optional<Color> c);
		void restore();
		void begin();
		void end();

	private:
		struct Impl;
		std::unique_ptr<Impl> m_Pimpl;
	};

}
