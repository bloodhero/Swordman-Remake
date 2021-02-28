#include "pch.h"
#include <SDL.h>
#include "Core/Window.h"
#include "Core/Manager.h"
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_KEYSTATE_BASED_INPUT

#include <nuklear.h>
#include "Nuklear/nuklear_sdl_gl3.inl"
#include "Nuklear/Nuklear.h"
#define MAX_VERTEX_MEMORY 512 * 1024
#define MAX_ELEMENT_MEMORY 128 * 1024

namespace meow {

	static SDL_Window* getSdlRenderer()
	{
		static SDL_Window* sdl_renderer = nullptr;
		if (sdl_renderer) return sdl_renderer;
		SdlWindow* sdl_window = static_cast<SdlWindow*>(Manager::getManager()->getWindow());
		sdl_renderer = static_cast<SDL_Window*>(sdl_window->getRawRenderer());
		return sdl_renderer;
	}

	struct SdlGl3Nuklear::Impl
	{
		struct nk_context* context;
		void begin();
		void end();
		void eventBegin();
		void eventEnd();
		void render();
		void onEvent(SDL_Event* event);
		void* getContext();
	};

	void SdlGl3Nuklear::Impl::begin()
	{
		context = nk_sdl_init(getSdlRenderer());
		{struct nk_font_atlas* atlas;
		nk_sdl_font_stash_begin(&atlas);
		/*struct nk_font *droid = nk_font_atlas_add_from_file(atlas, "../../../extra_font/DroidSans.ttf", 14, 0);*/
		/*struct nk_font *roboto = nk_font_atlas_add_from_file(atlas, "../../../extra_font/Roboto-Regular.ttf", 16, 0);*/
		/*struct nk_font *future = nk_font_atlas_add_from_file(atlas, "../../../extra_font/kenvector_future_thin.ttf", 13, 0);*/
		/*struct nk_font *clean = nk_font_atlas_add_from_file(atlas, "../../../extra_font/ProggyClean.ttf", 12, 0);*/
		/*struct nk_font *tiny = nk_font_atlas_add_from_file(atlas, "../../../extra_font/ProggyTiny.ttf", 10, 0);*/
		/*struct nk_font *cousine = nk_font_atlas_add_from_file(atlas, "../../../extra_font/Cousine-Regular.ttf", 13, 0);*/
		nk_sdl_font_stash_end();
		/*nk_style_load_all_cursors(ctx, atlas->cursors);*/
		/*nk_style_set_font(ctx, &roboto->handle);*/}
	}

	void SdlGl3Nuklear::Impl::end()
	{
		nk_sdl_shutdown();
	}

	void SdlGl3Nuklear::Impl::eventBegin()
	{
		nk_input_begin(context);
	}

	void SdlGl3Nuklear::Impl::eventEnd()
	{
		nk_input_end(context);
	}

	void SdlGl3Nuklear::Impl::render()
	{
		nk_sdl_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_MEMORY, MAX_ELEMENT_MEMORY);
	}

	void SdlGl3Nuklear::Impl::onEvent(SDL_Event* event)
	{
		nk_sdl_handle_event(event);
	}

	void* SdlGl3Nuklear::Impl::getContext()
	{
		return context;
	}

	SdlGl3Nuklear::SdlGl3Nuklear() :
		m_Pimpl(std::make_unique<Impl>())
	{

	}

	void SdlGl3Nuklear::begin()
	{
		m_Pimpl->begin();
	}


	void SdlGl3Nuklear::end()
	{
		m_Pimpl->end();
	}


	void SdlGl3Nuklear::eventBegin()
	{
		m_Pimpl->eventBegin();
	}


	void SdlGl3Nuklear::eventEnd()
	{
		m_Pimpl->eventEnd();
	}


	void SdlGl3Nuklear::render()
	{
		m_Pimpl->render();
	}


	void* SdlGl3Nuklear::getContext()
	{
		return m_Pimpl->getContext();
	}


	void SdlGl3Nuklear::onEvent(void* e)
	{
		auto event = static_cast<SDL_Event*>(e);
		m_Pimpl->onEvent(event);
	}

}
