#pragma once

#include <string>
#include <memory>
#include "Math/Vector2.h"


namespace meow {
	class Texture;
	class MixChunk;
	class Renderable;

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


	class Drawable
	{
	public:
		Drawable(Camera* cam) :m_camera(cam) {}
		virtual ~Drawable() = default;
		virtual void onDraw() = 0;
		virtual void onUpdate(float time) = 0;
		void setAnchorPoint(float x, float y) { m_AnchorPoint = { x,y }; }
		void setSize(int width, int height) { m_Size = { width, height }; }
		void setPosition(Vector2f pos) { m_Position = pos; }
		Rect getArea()
		{
			return {
				m_Position.x + m_Size.x * m_AnchorPoint.x,
				m_Position.y + m_Size.y * m_AnchorPoint.y,
				m_Size.x,
				m_Size.y
			};
		}

	protected:
		Vector2f m_Position;
		Vector2f m_AnchorPoint;
		Vector2f m_Size;
		Camera* m_camera;
	};


	class Animation :public Drawable
	{
	public:
		enum class AnimationType
		{
			PLAY_REPEAT,
			PLAY_ONCE,
			BACK_FORTH
		};

		// CREARTORS
		Animation(std::string_view json_file);
		~Animation() = default;

		// ACCESSORS
		bool isLooped();
		bool isEnd();
		void onDraw() override;


		// MANIPULATORS
		void onUpdate(float time) override;
		void setDesc(std::string_view desc);
		void setName(std::string_view name);
		void setLoop(bool loop);
		void setDuration(float time);
		void setDurationPerFrame(float time);
		void setType(AnimationType type);
		void pushGfxFrame(Renderable* gfx);
		void pushSfxFrame(int key_frame, MixChunk* sfx);
		void skip();

	private:
		struct Impl;
		std::unique_ptr<Impl*> m_Pimpl;

	};

	class Picture :public Drawable
	{
	public:
		Picture(Renderable* r);
		void onDraw() override;
		void onUpdate(float time) override {}

	private:
		struct Impl;
		std::unique_ptr<Impl*> m_Pimpl;
	};
}
