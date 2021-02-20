#include "pch.h"
#include <SDL.h>
#include "Core/Window.h"
#include "Core/Manager.h"
#include "Renderer/GfxDevice.h"
#include "Renderer/Camera.h"

namespace meow {

	static SDL_Renderer* getSdlRenderer()
	{
		static SDL_Renderer* sdl_renderer = nullptr;
		if (sdl_renderer) return sdl_renderer;
		SdlWindow* sdl_window = static_cast<SdlWindow*>(Manager::getManager()->getWindow());
		sdl_renderer = static_cast<SDL_Renderer*>(sdl_window->getRawRenderer());
		return sdl_renderer;
	}

	void SdlGfxDevice::setLogicalSize(Vector2i size)
	{
		SDL_RenderSetLogicalSize(getSdlRenderer(), size.x, size.y);
		Camera::setSize(size);
	}


	void SdlGfxDevice::setClearColor(Color color)
	{
		SDL_SetRenderDrawColor(getSdlRenderer(), color.r, color.g, color.b, color.a);
	}

	meow::Color SdlGfxDevice::getClearColor()
	{
		Uint8 r, g, b, a;
		SDL_GetRenderDrawColor(getSdlRenderer(), &r, &g, &b, &a);
		return { r, g, b, a };
	}


	meow::Vector2i SdlGfxDevice::getLogicalSize()
	{
		int w, h;
		SDL_RenderGetLogicalSize(getSdlRenderer(), &w, &h);
		return { w, h };
	}


	void SdlGfxDevice::clearScreen()
	{
		SDL_RenderClear(getSdlRenderer());
	}


	void SdlGfxDevice::updateScreen()
	{
		SDL_RenderPresent(getSdlRenderer());
	}


	void SdlGfxDevice::setScaleQuality(ScaleQuality qlt)
	{
		switch (qlt)
		{
		case meow::GfxDevice::ScaleQuality::NEAREST:
			SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
			break;
		case meow::GfxDevice::ScaleQuality::LINEAR:
			SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
			break;
		case meow::GfxDevice::ScaleQuality::BEST:
			SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
			break;
		default:
			break;
		}
	}

}
