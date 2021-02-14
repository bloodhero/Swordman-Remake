#include "pch.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <SDL.h>

#include "Core/Utils.h"
#include "Renderer/Texture.h"
#include "Core/Window.h"

namespace meow {

	static SDL_Renderer* getSdlRenderer()
	{
		static SDL_Renderer* sdl_renderer = nullptr;
		if (sdl_renderer) return sdl_renderer;
		SdlWindow* sdl_window = static_cast<SdlWindow*>(Manager::getManager()->getWindow());
		sdl_renderer = static_cast<SDL_Renderer*>(sdl_window->getRawRenderer());
		return sdl_renderer;
	}

	static SDL_Surface* loadImage(std::string_view filename)
	{
		int req_format = STBI_rgb_alpha;
		int width, height, orig_format;
		unsigned char* data = stbi_load(filename.data(), &width, &height, &orig_format, req_format);
		ASSERT(data, "Loading image failed: {}", filename.data());

		Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		rmask = 0xff000000;
		gmask = 0x00ff0000;
		bmask = 0x0000ff00;
		amask = 0x000000ff;
#else // little endian, like x86
		rmask = 0x000000ff;
		gmask = 0x0000ff00;
		bmask = 0x00ff0000;
		amask = 0xff000000;
#endif
		int depth, pitch;
		depth = 32;
		pitch = 4 * width;

		SDL_Surface* surf = SDL_CreateRGBSurfaceFrom((void*)data, width, height, depth, pitch,
			rmask, gmask, bmask, amask);

		ASSERT(surf, "Creating surface failed: {}", SDL_GetError());
		stbi_image_free(data);
		return surf;
	}

	struct SdlImage::Impl
	{
		Impl(std::string_view filename);
		SDL_Texture* sdlTexture = nullptr;
		Vector2i size;
		std::string filename;
		~Impl();
	};

	SdlImage::Impl::~Impl()
	{
		SDL_DestroyTexture(sdlTexture);
	}

	SdlImage::Impl::Impl(std::string_view filename)
	{
		auto surf = loadImage(filename);
		sdlTexture = SDL_CreateTextureFromSurface(getSdlRenderer(), surf);
		SDL_FreeSurface(surf);
	}


	struct SdlCanvas::Impl
	{
		SDL_Texture* sdlTexture = nullptr;
		Vector2i size;
		void pushLayer(Renderable* ra, Vector2i pos);
		~Impl();
	};

	SdlCanvas::Impl::~Impl()
	{
		SDL_DestroyTexture(sdlTexture);
	}

	void SdlCanvas::Impl::pushLayer(Renderable* ra, Vector2i pos)
	{
		SDL_SetRenderTarget(getSdlRenderer(), sdlTexture);

		auto sdl_tex = static_cast<SDL_Texture*>(ra->texture->getRawData());

		SDL_Rect src_rect = { ra->slice.x, ra->slice.y, ra->slice.w, ra->slice.h };
		auto tex_size = ra->texture->getSize();
		SDL_Rect dst_rect = { pos.x, pos.y, tex_size.x, tex_size.y };

		SDL_RenderCopy(getSdlRenderer(), sdl_tex, &src_rect, &dst_rect);

		SDL_SetRenderTarget(getSdlRenderer(), NULL);
	}

	SdlImage::SdlImage(std::string_view filename) :
		m_Pimpl(std::make_unique<Impl>(filename))
	{
	}

	void* SdlImage::getRawData()
	{
		return m_Pimpl->sdlTexture;
	}


	meow::Vector2i SdlImage::getSize()
	{
		return m_Pimpl->size;
	}

	SdlCanvas::SdlCanvas(int width, int height) :
		m_Pimpl(std::make_unique<Impl>())
	{
		ASSERT(width > 0 && height > 0, "Function[SdlCanvas::SdlCanvas]: width and height must greater than 0");
		m_Pimpl->sdlTexture = SDL_CreateTexture(getSdlRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
	}

	void* SdlCanvas::getRawData()
	{
		return m_Pimpl->sdlTexture;
	}


	meow::Vector2i SdlCanvas::getSize()
	{
		return m_Pimpl->size;
	}


	void SdlCanvas::pushLayer(Renderable* ra, Vector2i pos)
	{
		m_Pimpl->pushLayer(ra, pos);
	}

}
