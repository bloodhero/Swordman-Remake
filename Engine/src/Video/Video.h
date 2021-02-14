#pragma once

#include <string>
#include <memory>
#include "Renderer/Drawable.h"

namespace meow {
	class Video :public Drawable
	{
	public:
		virtual void skip() = 0;
		virtual bool isEnd() = 0;
		virtual std::string getName() = 0;
	};

	class FfmpegVideo :public Video
	{
	public:
		void skip() override;
		bool isEnd() override;
		void onDraw() override;
		void onUpdate(float time) override;
		void setPosition(Vector2f pos) override;
		Rect getArea() override;
		std::string getName() override;

	private:
		struct Impl;
		std::unique_ptr<Impl> m_Pimpl;
	};
}
