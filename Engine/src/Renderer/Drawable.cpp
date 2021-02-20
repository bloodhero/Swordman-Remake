#include "pch.h"
#include <SDL.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
#include <fstream>
#include "Renderer/Drawable.h"
#include "Core/Manager.h"
#include "Audio/Audio.h"
#include "Core/Window.h"
#include "Renderer/Texture.h"
#include "Renderer/Camera.h"
#include "Core/Utils.h"

namespace meow {

	static Rect getOffset(Rect object, Rect camera)
	{
		Vector2i object_pos = { object.x, object.y };
		Vector2i object_size = { object.w, object.h };
		Vector2i camera_pos = { camera.x, camera.y };
		Vector2i offset = object_pos - camera_pos;
		return { offset.x, offset.y, object_size.x, object_size.y };
	}


	static SDL_Rect rect2SDL_Rect(Rect r)
	{
		return { r.x, r.y, r.w, r.h };
	}

	static bool hasIntersection(Rect r1, Rect r2)
	{
		return SDL_HasIntersection(&rect2SDL_Rect(r1), &rect2SDL_Rect(r2)) == SDL_TRUE;
	}

	static SDL_Renderer* getSdlRenderer()
	{
		static SDL_Renderer* sdl_renderer = nullptr;
		if (sdl_renderer) return sdl_renderer;
		SdlWindow* sdl_window = static_cast<SdlWindow*>(Manager::getManager()->getWindow());
		sdl_renderer = static_cast<SDL_Renderer*>(sdl_window->getRawRenderer());
		return sdl_renderer;
	}


	// -------------------------------------- Drawable Interface Impl -------------------------------------
	struct DrawableImpl
	{
		Vector2f pos = { 0.f, 0.f };
		Vector2f anchor = { 0.f, 0.f };
		Vector2i size = { 0.f, 0.f };
		Rect getArea();
		void setSize(Vector2i size);
		void setAnchorPoint(Vector2f anchor);
		void setPosition(Vector2f pos);
	};


	meow::Rect DrawableImpl::getArea()
	{
		ASSERT(size.x > 0 && size.y > 0, "{}: must specify drawable size", HZ_FUNC_SIG);
		Vector2f size_f = { static_cast<float>(size.x), static_cast<float>(size.y) };
		Vector2f center = { size_f.x * anchor.x, size_f.y * anchor.y };
		Vector2f real_pos = pos - center;
		return {
			static_cast<int>(real_pos.x),
			static_cast<int>(real_pos.y),
			size.x,
			size.y
		};
	}


	void DrawableImpl::setSize(Vector2i size)
	{
		this->size = size;
	}


	void DrawableImpl::setAnchorPoint(Vector2f anchor)
	{
		this->anchor = anchor;
	}


	void DrawableImpl::setPosition(Vector2f pos)
	{
		this->pos = pos;
	}


	// -------------------------------------- Animation Impl -------------------------------------------
	struct Animation::Impl
	{
		// Impl
		std::string name = "";
		std::string desc = "";
		AnimationType type = AnimationType::NORMAL;
		DrawableImpl drawImpl;
		Camera* camera;
		int duration_per_frame = 0;
		bool isLooped = false;
		bool isEnd = false;
		int frameTotal = 0;
		int currentFrame = 0;
		int currentIndex = 0;
		float currentIndexF = 0.0f;
		std::vector<Renderable> gfxArray;
		std::vector<int> IndexArray;
		std::unordered_map<int, MixChunk*> sfxArray;
		float speed = 1.0f;
		int timesPlayed = 0;
		int keyFrame = -1;

		void setPosition(Vector2f pos) { drawImpl.setPosition(pos); }
		Rect getArea() { return drawImpl.getArea(); }
		void setAnchorPoint(Vector2f anchor) { drawImpl.setAnchorPoint(anchor); }
		void setSize(Vector2i size) { drawImpl.setSize(size); }
		bool isFinished() { return isEnd; }
		void setType(AnimationType type) { this->type = type; }
		void setName(std::string_view name) { this->name = name; }
		void setDesc(std::string_view desc) { this->desc = desc; }
		void setDurationPerFrame(int time) { duration_per_frame = time; }
		void setSpeed(float speed) { this->speed = speed; }
		void setCamera(Camera* cam) { camera = cam; }
		void setLoop(bool loop) { isLooped = loop; }

		void onDraw();
		void onUpdate(float time);
		void skip();
		void reset();
		void pushGfxFrame(Renderable r);
		void pushSfxFrame(int key_frame, MixChunk* sfx);
		void setUp();
		void syncTo(Animation* other);

		int getKeyFrame();
	};


	int Animation::Impl::getKeyFrame()
	{
		int temp = keyFrame;
		keyFrame = -1;
		return temp;
	}


	void Animation::Impl::pushSfxFrame(int key_frame, MixChunk* sfx)
	{
		if (sfxArray.find(key_frame) == sfxArray.end())
			sfxArray[key_frame] = sfx;
	}

	void Animation::Impl::skip()
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


	void Animation::Impl::setUp()
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

	void Animation::Impl::reset()
	{
		// set state to begin
		currentFrame = 0;
		currentIndex = 0;
		currentIndexF = 0.f;
		timesPlayed = 0;
		isEnd = false;
		keyFrame = -1;
	}

	void Animation::Impl::onDraw()
	{

		if (hasIntersection(this->getArea(),camera->getArea()) &&
			!gfxArray.empty() &&
			currentFrame >= 0)
		{

			Renderable ra = gfxArray[currentFrame];

			if (ra.alphaMod.has_value())
				ra.texture->setAlphaMod(ra.alphaMod.value());
			if (ra.colorMod.has_value())
				ra.texture->setColorMod(ra.colorMod.value());
			if (ra.blendMode.has_value())
				ra.texture->setBlendMode(ra.blendMode.value());

			Rect src_rect;
			Vector2i size = ra.texture->getSize();
			if (ra.slice.has_value())
				src_rect = ra.slice.value();
			else
				src_rect = { 0, 0, size.x, size.y };


			ra.texture->draw(src_rect, getOffset(this->getArea(), camera->getArea()));

			// reset state
			ra.texture->setAlphaMod(1.0f);
			ra.texture->setBlendMode(Renderable::BlendMode::BLEND);
			ra.texture->setColorMod({ 255,255,255,255 });

			int keyframe = getKeyFrame();
			if (sfxArray.find(keyframe) != sfxArray.end())
			{
				Manager::getManager()->getAudio()->playSFX(sfxArray[keyframe]);
			}
		}
	}

	void Animation::Impl::onUpdate(float time)
	{
		if ((type == AnimationType::PLAY_ONCE) &&
			(timesPlayed > 0) &&
			(isLooped == true))
		{
			currentFrame = frameTotal - 1;
		}
		else if ((timesPlayed > 0) && (isLooped == false))
		{
			currentFrame = -1;
		}
		else
		{
			currentIndexF += time * speed;

			while (currentIndexF >= IndexArray.size())
			{
				currentIndexF -= IndexArray.size();
				++timesPlayed;
			}

			currentIndex = static_cast<int>(currentIndexF);
			int current_frame = IndexArray[currentIndex];

			// 如果发生帧数发生变化，更新关键帧。
			if (currentFrame != current_frame)
			{
				keyFrame = current_frame;
			}

			currentFrame = current_frame;
		}
	}


	void Animation::Impl::pushGfxFrame(Renderable r)
	{
		gfxArray.push_back(r);
	}

	void Animation::Impl::syncTo(Animation* other)
	{
		Animation* sdl_other = static_cast<Animation*>(other);
		sdl_other->m_Pimpl->currentFrame = currentFrame;
		sdl_other->m_Pimpl->currentIndex = currentIndex;
		sdl_other->m_Pimpl->currentIndexF = currentIndexF;
		sdl_other->m_Pimpl->isEnd = isEnd;
		sdl_other->m_Pimpl->keyFrame = keyFrame;
	}

	Animation::Animation(std::string_view json_file, Camera* cam)
		:m_Pimpl(std::make_unique<Impl>())
	{
		auto dir_name = dirName(json_file.data());
		m_Pimpl->camera = cam;
		m_Pimpl->drawImpl.setSize({ 320, 240 });
		m_Pimpl->setAnchorPoint({ 0.5f, 0.5f });


		LOGGER->trace("path is {}", SDL_GetBasePath());

		// 读取
		using json = nlohmann::json;
		// read a JSON file
		std::ifstream i(json_file.data());
		json j;
		i >> j;

		// 解析
		int keyframe = 0;
		for (auto& e : j)
		{
			Shared<int> big;
			auto canvas = CreateShared <SdlCanvas>(320, 240);
			for (auto& l : e["layers"])
			{
				auto image = CreateShared<SdlImage>(dir_name + l["file"].get<std::string>());
				Renderable ra;
				ra.texture = image;
				int x = l["x"].get<int>();
				int y = l["y"].get<int>();
				canvas->pushLayer(ra, { x, y });
			}


			Renderable ra;
			ra.texture = canvas;
			m_Pimpl->pushGfxFrame(ra);

			if (e["sound"] != nullptr)
			{
				OpenALMixChunk* chunk = new OpenALMixChunk(dir_name + e["sound"].get<std::string>());
				m_Pimpl->pushSfxFrame(keyframe, chunk);
			}

			++keyframe;
		}

		m_Pimpl->isLooped = true;
		m_Pimpl->setDurationPerFrame(100);
		m_Pimpl->type = Animation::AnimationType::PLAY_ONCE;
		m_Pimpl->setUp();

	}


	Animation::Animation()
		:m_Pimpl(std::make_unique<Impl>())
	{

	}


	void Animation::setType(AnimationType type)
	{
		m_Pimpl->setType(type);
	}


	void Animation::setName(std::string_view name)
	{
		m_Pimpl->setName(name);
	}


	void Animation::setDesc(std::string_view desc)
	{
		m_Pimpl->setDesc(desc);
	}


	void Animation::setDurationPerFrame(int time)
	{
		m_Pimpl->setDurationPerFrame(time);
	}

	void Animation::onDraw()
	{
		m_Pimpl->onDraw();
	}

	void Animation::onUpdate(float time)
	{
		m_Pimpl->onUpdate(time);
	}

	void Animation::reset()
	{
		m_Pimpl->reset();
	}


	void Animation::skip()
	{
		m_Pimpl->skip();
	}


	void Animation::setSpeed(float speed)
	{
		m_Pimpl->setSpeed(speed);
	}

	void Animation::pushGfxFrame(Renderable r)
	{
		m_Pimpl->pushGfxFrame(r);
	}


	void Animation::pushSfxFrame(int key_frame, MixChunk* sfx)
	{
		m_Pimpl->pushSfxFrame(key_frame, sfx);
	}


	void Animation::setUp()
	{
		m_Pimpl->setUp();
	}


	void Animation::setCamera(Camera* cam)
	{
		m_Pimpl->setCamera(cam);
	}


	void Animation::setSize(Vector2i size)
	{
		m_Pimpl->setSize(size);
	}

	void Animation::setAnchorPoint(Vector2f anchor)
	{
		m_Pimpl->setAnchorPoint(anchor);
	}

	void Animation::setLoop(bool loop)
	{
		m_Pimpl->setLoop(loop);
	}


	void Animation::syncTo(Animation* other)
	{
		m_Pimpl->syncTo(other);
	}

	bool Animation::isEnd()
	{
		return m_Pimpl->isFinished();
	}

	void Animation::setPosition(Vector2f pos)
	{
		m_Pimpl->setPosition(pos);
	}

	meow::Rect Animation::getArea()
	{
		return m_Pimpl->getArea();
	}


	//-------------------------------- Picture Impl -----------------------------------

	struct Picture::Impl
	{
		Impl(std::string_view filename, Camera* cam);
		Impl(Renderable ra, Camera* cam);
		~Impl() = default;
		Animation animation;
	};

	Picture::Impl::Impl(std::string_view filename, Camera* cam)
	{
		auto image = CreateShared<SdlImage>(filename);
		Renderable ra;
		ra.texture = image;
		animation.setSize(image->getSize());
		animation.setName(filename);
		animation.setDurationPerFrame(200);
		animation.setLoop(true);
		animation.setPosition({ 0.f, 0.f });
		animation.setType(Animation::AnimationType::NORMAL);
		animation.setCamera(cam);
		animation.pushGfxFrame(ra);
		animation.setUp();
	}

	Picture::Impl::Impl(Renderable ra, Camera* cam)
	{
		animation.setSize(ra.texture->getSize());
		animation.setDurationPerFrame(200);
		animation.setLoop(true);
		animation.setPosition({ 0.f, 0.f });
		animation.setType(Animation::AnimationType::NORMAL);
		animation.setCamera(cam);
		animation.pushGfxFrame(ra);
		animation.setUp();
	}


	Picture::Picture(std::string_view filename, Camera* cam) :
		m_Pimpl(std::make_unique<Impl>(filename, cam))
	{

	}


	Picture::Picture(Renderable ra, Camera* cam) :
		m_Pimpl(std::make_unique<Impl>(ra, cam))
	{

	}

	Picture::~Picture()
	{

	}

	void Picture::onDraw()
	{
		m_Pimpl->animation.onDraw();
	}

	void Picture::setPosition(Vector2f pos)
	{
		m_Pimpl->animation.setPosition(pos);
	}

	meow::Rect Picture::getArea()
	{
		return m_Pimpl->animation.getArea();
	}


	void Picture::setAnchorPoint(Vector2f anchor)
	{
		m_Pimpl->animation.setAnchorPoint(anchor);
	}


	void Picture::onUpdate(float time)
	{
		m_Pimpl->animation.onUpdate(time);
	}


	void Picture::setSize(Vector2i size)
	{
		m_Pimpl->animation.setSize(size);
	}

}
