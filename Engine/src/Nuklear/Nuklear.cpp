#include "pch.h"
#include <SDL.h>
#include "Core/Window.h"
#include "Core/Manager.h"
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_INCLUDE_SOFTWARE_FONT
#define NK_BUTTON_TRIGGER_ON_RELEASE

#include <nuklear.h>
#include "Nuklear/nuklear_sdl.inl"
#include "Nuklear/Nuklear.h"

namespace meow {

	static SDL_Renderer* getSdlRenderer()
	{
		static SDL_Renderer* sdl_renderer = nullptr;
		if (sdl_renderer) return sdl_renderer;
		SdlWindow* sdl_window = static_cast<SdlWindow*>(Manager::getManager()->getWindow());
		sdl_renderer = static_cast<SDL_Renderer*>(sdl_window->getRawRenderer());
		return sdl_renderer;
	}

	static nk_keys translate_sdl_key(struct SDL_Keysym const* k)
	{
		/*keyboard handling left as an exercise for the reader */

		return NK_KEY_NONE;
	}


	static nk_buttons sdl_button_to_nk(int button)
	{
		switch (button)
		{
		case SDL_BUTTON_LEFT:
			return NK_BUTTON_LEFT;
			break;
		case SDL_BUTTON_MIDDLE:
			return NK_BUTTON_MIDDLE;
			break;
		case SDL_BUTTON_RIGHT:
			return NK_BUTTON_RIGHT;
			break;
		default:
			//ft
			break;
		}
	}


	struct SdlSurfaceNuklear::Impl
	{
		SDL_Surface* surface;
		struct sdlsurface_context* context;
		struct nk_color clear = { 0,0,0,0 };
		struct nk_vec2 vec;
		void begin();
		void end();
		void eventBegin();
		void eventEnd();
		void render();
		void onEvent(SDL_Event& event);
		void* getContext();
	};

	void SdlSurfaceNuklear::Impl::begin()
	{
		auto size = Manager::getManager()->getGfxDevice()->getLogicalSize();
		surface = SDL_CreateRGBSurfaceWithFormat(0, size.x, size.y, 32, SDL_PIXELFORMAT_ARGB8888);
		context = nk_sdlsurface_init(surface, 13.0f);
	}

	void SdlSurfaceNuklear::Impl::end()
	{
		nk_sdlsurface_shutdown(context);
		SDL_FreeSurface(surface);
	}

	void SdlSurfaceNuklear::Impl::eventBegin()
	{
		nk_input_begin(&(context->ctx));
	}

	void SdlSurfaceNuklear::Impl::eventEnd()
	{
		nk_input_end(&(context->ctx));
	}

	void SdlSurfaceNuklear::Impl::render()
	{

		nk_sdlsurface_render(context, clear, 1);
		SDL_Texture* tex = SDL_CreateTextureFromSurface(getSdlRenderer(), surface);
		SDL_RenderCopy(getSdlRenderer(), tex, NULL, NULL);
		SDL_DestroyTexture(tex);
	}

	void SdlSurfaceNuklear::Impl::onEvent(SDL_Event& event)
	{
		switch (event.type)
		{
		case SDL_QUIT:
			exit(0);
			break;
		case SDL_KEYDOWN:
			nk_input_key(&(context->ctx), translate_sdl_key(&event.key.keysym), 1);
			break;
		case SDL_KEYUP:
			nk_input_key(&(context->ctx), translate_sdl_key(&event.key.keysym), 0);
			break;
		case SDL_MOUSEMOTION:
			nk_input_motion(&(context->ctx), event.motion.x, event.motion.y);
			break;
		case SDL_MOUSEBUTTONDOWN:
			nk_input_button(&(context->ctx), sdl_button_to_nk(event.button.button), event.button.x, event.button.y, 1);
			break;
		case SDL_MOUSEBUTTONUP:
			nk_input_button(&(context->ctx), sdl_button_to_nk(event.button.button), event.button.x, event.button.y, 0);
			break;
		case SDL_MOUSEWHEEL:
			vec.x = event.wheel.x;
			vec.y = event.wheel.y;
			nk_input_scroll(&(context->ctx), vec);
			break;
		}
	}

	void* SdlSurfaceNuklear::Impl::getContext()
	{
		return &context->ctx;
	}

	SdlSurfaceNuklear::SdlSurfaceNuklear() :
		m_Pimpl(std::make_unique<Impl>())
	{

	}

	void SdlSurfaceNuklear::begin()
	{
		m_Pimpl->begin();
	}


	void SdlSurfaceNuklear::end()
	{
		m_Pimpl->end();
	}


	void SdlSurfaceNuklear::eventBegin()
	{
		m_Pimpl->eventBegin();
	}


	void SdlSurfaceNuklear::eventEnd()
	{
		m_Pimpl->eventEnd();
	}


	void SdlSurfaceNuklear::render()
	{
		m_Pimpl->render();
	}


	void* SdlSurfaceNuklear::getContext()
	{
		return m_Pimpl->getContext();
	}


	void SdlSurfaceNuklear::onEvent(void* e)
	{
		auto event = static_cast<SDL_Event*>(e);
		m_Pimpl->onEvent(*event);
	}

}
