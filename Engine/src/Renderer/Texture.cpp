#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <SDL.h>
#include <memory>

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
		int x, y, n;
		unsigned char* data = stbi_load(filename.data(), &x, &y, &n, 0);

		// do sth

		stbi_image_free(data);
	}

	struct SdlImage::Impl
	{
		SDL_Texture* sdlTexture = nullptr;
		Vector2i size;
		~Impl();
	};

	SdlImage::Impl::~Impl()
	{
		SDL_DestroyTexture(sdlTexture);
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
		m_Pimpl(std::make_unique<Impl>())
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

}
