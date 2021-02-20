#include "pch.h"
#include "Renderer/PostEffect.h"
#include <SDL.h>

namespace meow {

	static SDL_Renderer* getSdlRenderer()
	{
		static SDL_Renderer* sdl_renderer = nullptr;
		if (sdl_renderer) return sdl_renderer;
		SdlWindow* sdl_window = static_cast<SdlWindow*>(Manager::getManager()->getWindow());
		sdl_renderer = static_cast<SDL_Renderer*>(sdl_window->getRawRenderer());
		return sdl_renderer;
	}


	struct SdlPostEffect::Impl
	{
		Impl() = default;
		~Impl();
		Vector2i size = Manager::getManager()->getGfxDevice()->getLogicalSize();
		Rect sliceOrig = {
			0, 0,
			size.x,
			size.y };
		Color colorModOrig = { 255,255,255,255 };
		float alphaModOrig = 1.0f;

		Rect slice = sliceOrig;
		Color colorMod = colorModOrig;
		float alphaMod = alphaModOrig;

		SDL_Texture* texture = SDL_CreateTexture(
			getSdlRenderer(),
			SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_TARGET,
			size.x,
			size.y);

		void setSlice(Rect s);
		void setAlphaMod(float a);
		void setColorMod(Color c);
		void restore();
		void begin();
		void end();
	};

	void SdlPostEffect::Impl::setSlice(Rect s)
	{
		slice = s;
	}

	void SdlPostEffect::Impl::setAlphaMod(float a)
	{
		alphaMod = a;
	}

	void SdlPostEffect::Impl::setColorMod(Color c)
	{
		colorMod = c;
	}

	void SdlPostEffect::Impl::restore()
	{
		slice = sliceOrig;
		alphaMod = alphaModOrig;
		colorMod = colorModOrig;
	}

	void SdlPostEffect::Impl::begin()
	{
		SDL_RenderClear(getSdlRenderer());
		SDL_SetRenderTarget(getSdlRenderer(), texture);
	}

	void SdlPostEffect::Impl::end()
	{
		SDL_RenderPresent(getSdlRenderer());
		SDL_SetRenderTarget(getSdlRenderer(), NULL);
		SDL_SetTextureAlphaMod(texture, (Uint8)alphaMod * 255);
		SDL_SetTextureColorMod(texture, colorMod.r, colorMod.g, colorMod.b);
		SDL_Rect src_rect = { slice.x, slice.y, slice.w, slice.h };
		SDL_RenderCopy(getSdlRenderer(), texture, &src_rect, NULL);
	}

	SdlPostEffect::Impl::~Impl()
	{
		SDL_DestroyTexture(texture);
	}

	SdlPostEffect::~SdlPostEffect()
	{

	}

	void SdlPostEffect::setSlice(Rect s)
	{
		m_Pimpl->setSlice(s);
	}


	void SdlPostEffect::setAlphaMod(float a)
	{
		m_Pimpl->setAlphaMod(a);
	}


	void SdlPostEffect::setColorMod(Color c)
	{
		m_Pimpl->setColorMod(c);
	}


	void SdlPostEffect::restore()
	{
		m_Pimpl->restore();
	}


	void SdlPostEffect::begin()
	{
		m_Pimpl->begin();
	}


	void SdlPostEffect::end()
	{
		m_Pimpl->end();
	}

}
