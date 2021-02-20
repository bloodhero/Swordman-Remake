#pragma once

#include <string>
#include <memory>
#include "Math/Vector2.h"
#include "Renderer/Renderable.h"
#include "Core/Utils.h"


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
		virtual void setAnchorPoint(Vector2f anchor) = 0;
		virtual Rect getArea() = 0;
		virtual void setSize(Vector2i size) = 0;
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
		Animation();
		Animation(std::string_view json_file, Camera* cam);
		void setType(AnimationType type);
		void setName(std::string_view name);
		void setDesc(std::string_view desc);
		void setDurationPerFrame(int time);
		void setPosition(Vector2f pos);
		void setAnchorPoint(Vector2f anchor);
		void setSize(Vector2i size);
		void pushGfxFrame(Renderable r);
		void pushSfxFrame(int key_frame, MixChunk* sfx);
		void setCamera(Camera* cam);
		void setLoop(bool loop);
		void setUp();

		// ACCESSORS
		Rect getArea();
		bool isEnd();
		void syncTo(Animation* other);

		// MANIPULATORS
		void onDraw();
		void onUpdate(float time);
		void skip();
		void setSpeed(float speed);
		void reset();


	private:

		struct Impl;
		std::unique_ptr<Impl> m_Pimpl;
	};


	class Picture :public Drawable
	{
	public:
		Picture(std::string_view filename, Camera* cam);
		Picture(Renderable ra, Camera* cam);
		~Picture();
		void onDraw();
		void setPosition(Vector2f pos);
		Rect getArea();
		void setAnchorPoint(Vector2f anchor);
		void onUpdate(float time);
		void setSize(Vector2i size);

	private:

		struct Impl;
		std::unique_ptr<Impl> m_Pimpl;
	};
}
