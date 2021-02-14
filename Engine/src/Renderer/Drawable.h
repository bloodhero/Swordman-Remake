#pragma once

#include <string>
#include <memory>
#include "Math/Vector2.h"
#include "Renderer/Renderable.h"



namespace meow {
	class Texture;
	class MixChunk;
	class Camera;


	class Drawable
	{
	public:
		virtual ~Drawable() = default;
		virtual void onDraw() = 0;
		virtual void onUpdate(float time) = 0;
		virtual void setPosition(Vector2f pos) = 0;
		virtual Rect getArea() = 0;
	};


	class Animation :public Drawable
	{
	public:
		enum class AnimationType
		{
			NORMAL,
			PLAY_ONCE,
			BACK_FORTH
		};

		// CREARTORS
		~Animation() = default;
		virtual bool isEnd() = 0;
		virtual void skip() = 0;
		virtual void reset() = 0;
	};

	class Picture :public Drawable
	{
	public:
		void onUpdate(float time) override {}
		virtual void setAnchorPoint(float x, float y) = 0;

	};

	class SdlAnimation :public Animation
	{
	public:
		SdlAnimation(std::string_view json_file, Camera* cam);
		void onDraw() override;
		void onUpdate(float time) override;
		void setPosition(Vector2f pos) override;
		Rect getArea() override;
		bool isEnd() override;
		void skip() override;
		void reset() override;

	private:
		struct Impl;
		std::unique_ptr<Impl> m_Pimpl;
	};

	class SdlPicture :public Picture
	{
	public:
		void onDraw() override;
		void setPosition(Vector2f pos) override;
		Rect getArea() override;
		void setAnchorPoint(float x, float y) override;

	private:
		struct Impl;
		std::unique_ptr<Impl> m_Pimpl;
	};
}
