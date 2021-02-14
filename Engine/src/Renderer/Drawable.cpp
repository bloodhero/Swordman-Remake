#include "pch.h"
#include <SDL.h>
#include <nlohmann/json.hpp>

#include "Renderer/Drawable.h"
#include "Core/Manager.h"
#include "Audio/Audio.h"
#include "Core/Window.h"
#include "Renderer/Renderable.h"
#include "Renderer/Texture.h"
#include "Renderer/Camera.h"

namespace meow {

	static SDL_Rect Rect2SDL_Rect(Rect r)
	{
		return { r.x, r.y, r.w, r.h };
	}

	static SDL_Renderer* getSdlRenderer()
	{
		static SDL_Renderer* sdl_renderer = nullptr;
		if (sdl_renderer) return sdl_renderer;
		SdlWindow* sdl_window = static_cast<SdlWindow*>(Manager::getManager()->getWindow());
		sdl_renderer = static_cast<SDL_Renderer*>(sdl_window->getRawRenderer());
		return sdl_renderer;
	}

	// -------------------------------------- SdlAnimation Impl -------------------------------------------
	struct SdlAnimation::Impl
	{
		// Drawable
		Vector2f position;
		Vector2f anchorPoint;
		Vector2f size;
		Camera* camera = nullptr;
		void onDraw();
		void onUpdate(float time);

		// Animation
		bool isLooped = false;
		bool isEnd = false;
		std::string name = "";
		std::string desc = "";
		int duration_per_frame = 0;
		AnimationType type = AnimationType::NORMAL;
		void pushGfxFrame(Renderable* r);
		void pushSfxFrame(int key_frame, MixChunk* sfx);
		void skip();
		void reset();

		// Impl
		int frameTotal = 0;
		int currentFrame = 0;
		int currentIndex = 0;
		float currentIndexF = 0.0f;
		std::vector<Renderable*> gfxArray;
		std::vector<int> IndexArray;
		std::unordered_map<int, MixChunk*> sfxArray;
		float speed = 1.0f;
		int timesPlayed = 0;
		int keyFrame = -1;
		int getKeyFrame();
		void setUp();
		Rect getArea();

	};

	int SdlAnimation::Impl::getKeyFrame()
	{
		int temp = keyFrame;
		keyFrame = -1;
		return temp;
	}



	void SdlAnimation::Impl::pushSfxFrame(int key_frame, MixChunk* sfx)
	{
		if (sfxArray.find(keyFrame) == sfxArray.end())
			sfxArray[keyFrame] = sfx;
	}

	void SdlAnimation::Impl::skip()
	{
		// set state to end
		if (isLooped == false)
		{
			currentFrame = frameTotal - 1;
			currentIndex = IndexArray.size() - 1;
			currentIndexF = IndexArray.size() - 1;
			timesPlayed = 1;
			isEnd = true;
			keyFrame = -1;
		}
	}


	void SdlAnimation::Impl::setUp()
	{

		if (!gfxArray.empty())
		{
			// 清空帧索引
			IndexArray.clear();

			// 添加帧索引
			for (int i = 0; i < gfxArray.size(); ++i)
				for (int j = 0; j < duration_per_frame; ++j)
					IndexArray.push_back(i);

			frameTotal = gfxArray.size();

			// 特殊处理 ANIMTYPE_BACK_FORTH 类型的动画
			if (type == AnimationType::BACK_FORTH)
			{
				for (int i = gfxArray.size() - 1; i > 0; --i)
					for (int j = 0; j < duration_per_frame; ++j)
						IndexArray.push_back(i);

				frameTotal *= 2;
			}

			// 重置动画
			reset();
		}

	}

	void SdlAnimation::Impl::reset()
	{
		// set state to begin
		currentFrame = 0;
		currentIndex = 0;
		currentIndexF = 0.f;
		timesPlayed = 0;
		isEnd = false;
		keyFrame = -1;
	}

	void SdlAnimation::Impl::onDraw()
	{

		SDL_Rect dst_rect = Rect2SDL_Rect(getArea());
		SDL_Rect cam_rect = Rect2SDL_Rect(camera->getArea());

		if (SDL_HasIntersection(&dst_rect, &cam_rect) == SDL_TRUE && !gfxArray.empty() && currentFrame >= 0)
		{
			auto ra = gfxArray[currentFrame];

			SDL_Rect src_rect = Rect2SDL_Rect(ra->slice);

			SDL_RenderCopy(
				getSdlRenderer(),
				static_cast<SDL_Texture*>(ra->texture->getRawData()),
				&src_rect,
				&dst_rect
			);


			int keyframe = getKeyFrame();
			if (sfxArray.find(keyframe) != sfxArray.end())
			{
				Manager::getManager()->getAudio()->playSFX(sfxArray[keyframe]);
			}
		}

	}

	void SdlAnimation::Impl::onUpdate(float time)
	{
		if ((type == AnimationType::PLAY_ONCE) &&
			(timesPlayed > 0) &&
			(isLooped == true))
		{
			currentFrame = frameTotal - 1;
		}
		else if ((type == AnimationType::PLAY_ONCE) &&
			(timesPlayed > 0) &&
			(isLooped == false))
		{
			currentFrame = -1;
		}
		else
		{
			currentIndexF = currentIndexF + time / 1000.f * speed;

			while (currentIndexF >= IndexArray.size())
			{
				currentIndexF -= IndexArray.size();
				++timesPlayed;
			}

			currentIndex = static_cast<int>(currentIndexF);
			auto current_frame = IndexArray[currentIndex];

			// 如果发生帧数发生变化，更新关键帧。
			if (currentFrame != current_frame)
			{
				keyFrame = current_frame;
			}

			currentFrame = current_frame;
		}
	}

	Rect SdlAnimation::Impl::getArea()
	{
		return { 0, 0, 0, 0 };
	}

	void SdlAnimation::Impl::pushGfxFrame(Renderable* r)
	{
		gfxArray.push_back(r);
	}

	void SdlAnimation::skip()
	{
		m_Pimpl->skip();
	}


	SdlAnimation::SdlAnimation(std::string_view json_file, Camera* cam)
	{
		m_Pimpl->camera = cam;
		m_Pimpl->pushGfxFrame(nullptr);
		m_Pimpl->pushSfxFrame(-1, nullptr);
		m_Pimpl->setUp();
	}

	void SdlAnimation::onDraw()
	{
		m_Pimpl->onDraw();
	}

	void SdlAnimation::onUpdate(float time)
	{
		m_Pimpl->onUpdate(time);
	}

	void SdlAnimation::reset()
	{
		m_Pimpl->reset();
	}


	void SdlAnimation::pushGfxFrame(Renderable* r)
	{
		m_Pimpl->pushGfxFrame(r);
	}


	void SdlAnimation::pushSfxFrame(int key_frame, MixChunk* sfx)
	{
		m_Pimpl->pushSfxFrame(key_frame, sfx);
	}

	bool SdlAnimation::isEnd()
	{
		return m_Pimpl->isEnd;
	}

	void SdlAnimation::setPosition(Vector2f pos)
	{
		m_Pimpl->position = pos;
	}

	meow::Rect SdlAnimation::getArea()
	{
		return m_Pimpl->getArea();
	}


	//-------------------------------- SdlPicture Impl -----------------------------------

	struct SdlPicture::Impl
	{
		Camera* camera;
		Vector2f position;
		Vector2f anchorPoint;
		Vector2f size;
	};

	void SdlPicture::onDraw()
	{

	}

	void SdlPicture::setPosition(Vector2f pos)
	{

	}

	meow::Rect SdlPicture::getArea()
	{
		return { 0, 0, 0, 0 };
	}


	void SdlPicture::setAnchorPoint(float x, float y)
	{

	}

}
