#include "pch.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <SDL.h>

#include "Core/Utils.h"
#include "Renderer/Texture.h"
#include "Core/Window.h"

namespace meow {

	static SDL_BlendMode convertBlendMode(Renderable::BlendMode mode)
	{
		switch (mode)
		{
		case meow::Renderable::BlendMode::BLEND:
			return SDL_BLENDMODE_BLEND;
			break;

		case meow::Renderable::BlendMode::ADD:
			return SDL_BLENDMODE_ADD;
			break;

		case meow::Renderable::BlendMode::MOD:
			return SDL_BLENDMODE_MOD;
			break;

		default:
			return SDL_BLENDMODE_NONE;
			break;
		}
	}


	static SDL_Renderer* getSdlRenderer()
	{
		static SDL_Renderer* sdl_renderer = nullptr;
		if (sdl_renderer) return sdl_renderer;
		SdlWindow* sdl_window = static_cast<SdlWindow*>(Manager::getManager()->getWindow());
		sdl_renderer = static_cast<SDL_Renderer*>(sdl_window->getRawRenderer());
		return sdl_renderer;
	}


	static SDL_Rect rect2SDL_Rect(Rect r)
	{
		return { r.x, r.y, r.w, r.h };
	}


	static void draw(SDL_Texture* tex, std::optional<Rect> src, std::optional<Rect> dst)
	{
		SDL_Rect src_rect;
		SDL_Rect dst_rect;
		SDL_Rect* src_rect_p;
		SDL_Rect* dst_rect_p;

		if (src.has_value())
		{
			src_rect = rect2SDL_Rect(src.value());
			src_rect_p = &src_rect;
		}
		else
		{
			src_rect_p = nullptr;
		}

		if (dst.has_value())
		{
			dst_rect = rect2SDL_Rect(dst.value());
			dst_rect_p = &dst_rect;
		}
		else
		{
			dst_rect_p = nullptr;
		}

		SDL_RenderCopy(getSdlRenderer(), tex, src_rect_p, dst_rect_p);
	}


	static void setBlendMode(SDL_Texture* tex, std::optional<Renderable::BlendMode> blend)
	{
		if (blend.has_value())
		{
			SDL_SetTextureBlendMode(tex, convertBlendMode(blend.value()));
		}
		else
		{
			SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
		}
	}


	static void setColorMod(SDL_Texture* tex, std::optional<Color> color)
	{
		if (color.has_value())
		{
			SDL_SetTextureColorMod(tex, color.value().r, color.value().g, color.value().b);
		}
		else
		{
			SDL_SetTextureColorMod(tex, 255, 255, 255);
		}
	}


	static void setAlphaMod(SDL_Texture* tex, std::optional<float> alpha)
	{
		if (alpha.has_value())
		{
			SDL_SetTextureAlphaMod(tex, static_cast<Uint8>(alpha.value() * 255));
		}
		else
		{
			SDL_SetTextureAlphaMod(tex, 255);
		}
	}


	struct SdlImage::Impl
	{
		Impl(std::string_view filename);
		~Impl();
		SDL_Texture* texture = nullptr;
		Vector2i size;
		std::string filename;
		void draw(std::optional<Rect> src, std::optional<Rect> dst) { meow::draw(texture, src, dst); }
		void setBlendMode(std::optional<Renderable::BlendMode> blend) { meow::setBlendMode(texture, blend); }
		void setColorMod(std::optional<Color> color) { meow::setColorMod(texture, color); }
		void setAlphaMod(std::optional<float> alpha) { meow::setAlphaMod(texture, alpha); }
	};


	SdlImage::Impl::~Impl()
	{
		SDL_DestroyTexture(texture);
	}


	SdlImage::Impl::Impl(std::string_view filename)
	{
		this->filename = filename;

		int req_format = STBI_rgb_alpha;
		int width, height, orig_format;
		unsigned char* data = stbi_load(filename.data(), &width, &height, &orig_format, req_format);
		ASSERT(data, "Loading image failed: {}", filename.data());

		int depth, pitch;
		depth = 32;
		pitch = 4 * width;

		SDL_Surface* surf = SDL_CreateRGBSurfaceWithFormatFrom((void*)data, width, height, depth, pitch,
			SDL_PIXELFORMAT_RGBA32);

		ASSERT(surf, "Creating surface failed: {}", SDL_GetError());

		size = { surf->w, surf->h };
		texture = SDL_CreateTextureFromSurface(getSdlRenderer(), surf);
		SDL_FreeSurface(surf);
		stbi_image_free(data);
	}


	SdlImage::SdlImage(std::string_view filename) :
		m_Pimpl(std::make_unique<Impl>(filename))
	{
	}


	void* SdlImage::getRawData()
	{
		return m_Pimpl->texture;
	}


	meow::Vector2i SdlImage::getSize()
	{
		return m_Pimpl->size;
	}


	void SdlImage::draw(std::optional<Rect> src, std::optional<Rect> dst)
	{
		m_Pimpl->draw(src, dst);
	}


	void SdlImage::setBlendMode(std::optional<Renderable::BlendMode> blend)
	{
		m_Pimpl->setBlendMode(blend);
	}


	void SdlImage::setColorMod(std::optional<Color> color)
	{
		m_Pimpl->setColorMod(color);
	}


	void SdlImage::setAlphaMod(std::optional<float> alpha)
	{
		m_Pimpl->setAlphaMod(alpha);
	}


	struct SdlCanvas::Impl
	{
		Impl(int width, int height);
		~Impl();
		SDL_Texture* texture = nullptr;
		Vector2i size;
		void pushLayer(Renderable ra, Vector2i pos);
		void draw(std::optional<Rect> src, std::optional<Rect> dst) { meow::draw(texture, src, dst); }
		void setBlendMode(std::optional<Renderable::BlendMode> blend) { meow::setBlendMode(texture, blend); }
		void setColorMod(std::optional<Color> color) { meow::setColorMod(texture, color); }
		void setAlphaMod(std::optional<float> alpha) { meow::setAlphaMod(texture, alpha); }
	};


	SdlCanvas::Impl::~Impl()
	{
		SDL_DestroyTexture(texture);
	}


	void SdlCanvas::Impl::pushLayer(Renderable ra, Vector2i pos)
	{
		auto temp = SDL_GetRenderTarget(getSdlRenderer());
		SDL_SetRenderTarget(getSdlRenderer(), texture);

		Vector2i size = ra.texture->getSize();
		Rect dst_rect;

		if (ra.slice.has_value())
		{
			dst_rect = { pos.x, pos.y, ra.slice.value().w, ra.slice.value().h };
		}
		else
		{
			dst_rect = { pos.x, pos.y, size.x, size.y };
		}

		ra.draw(dst_rect);
		Manager::getManager()->getGfxDevice()->updateScreen();
		SDL_SetRenderTarget(getSdlRenderer(), temp);
	}

	SdlCanvas::Impl::Impl(int width, int height) :
		size({width, height})
	{
		ASSERT(width > 0 && height > 0, "Function[SdlCanvas::SdlCanvas]: width and height must greater than 0");
		texture = SDL_CreateTexture(getSdlRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
	}


	SdlCanvas::SdlCanvas(int width, int height) :
		m_Pimpl(std::make_unique<Impl>(width, height))
	{

	}


	void* SdlCanvas::getRawData()
	{
		return m_Pimpl->texture;
	}


	meow::Vector2i SdlCanvas::getSize()
	{
		return m_Pimpl->size;
	}

	void SdlCanvas::pushLayer(Renderable ra, Vector2i pos)
	{
		m_Pimpl->pushLayer(ra, pos);
	}


	void SdlCanvas::draw(std::optional<Rect> src, std::optional<Rect> dst)
	{
		m_Pimpl->draw(src, dst);
	}


	void SdlCanvas::setBlendMode(std::optional<Renderable::BlendMode> blend)
	{
		m_Pimpl->setBlendMode(blend);
	}


	void SdlCanvas::setColorMod(std::optional<Color> color)
	{
		m_Pimpl->setColorMod(color);
	}


	void SdlCanvas::setAlphaMod(std::optional<float> alpha)
	{
		m_Pimpl->setAlphaMod(alpha);
	}


	struct SdlGlass::Impl
	{
		Impl(Color c);
		~Impl();
		SDL_Texture* texture;
		Vector2i size;
		void draw(std::optional<Rect> src, std::optional<Rect> dst) { meow::draw(texture, src, dst); }
		void setBlendMode(std::optional<Renderable::BlendMode> blend) { meow::setBlendMode(texture, blend); }
		void setColorMod(std::optional<Color> color) { meow::setColorMod(texture, color); }
		void setAlphaMod(std::optional<float> alpha) { meow::setAlphaMod(texture, alpha); }
	};


	SdlGlass::Impl::Impl(Color color)
	{
		size = Manager::getManager()->getGfxDevice()->getLogicalSize();
		texture = SDL_CreateTexture(
			getSdlRenderer(),
			SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_TARGET,
			size.x,
			size.y);

		// save state
		auto temp = SDL_GetRenderTarget(getSdlRenderer());
		Uint8 r, g, b, a;
		SDL_GetRenderDrawColor(getSdlRenderer(), &r, &g, &b, &a);

		// do sth on texture
		SDL_SetRenderTarget(getSdlRenderer(), texture);
		SDL_SetRenderDrawColor(getSdlRenderer(), color.r, color.g, color.b, color.a);
		SDL_RenderClear(getSdlRenderer());

		// restore state
		SDL_SetRenderDrawColor(getSdlRenderer(), r, g, b, a);
		SDL_SetRenderTarget(getSdlRenderer(), temp);
	}


	SdlGlass::Impl::~Impl()
	{
		SDL_DestroyTexture(texture);
	}


	SdlGlass::SdlGlass(Color color) :
		m_Pimpl(std::make_unique<Impl>(color))

	{

	}


	SdlGlass::~SdlGlass()
	{

	}

	void* SdlGlass::getRawData()
	{
		return m_Pimpl->texture;
	}


	meow::Vector2i SdlGlass::getSize()
	{
		return m_Pimpl->size;
	}


	void SdlGlass::draw(std::optional<Rect> src, std::optional<Rect> dst)
	{
		m_Pimpl->draw(src, dst);
	}


	void SdlGlass::setBlendMode(std::optional<Renderable::BlendMode> blend)
	{
		m_Pimpl->setBlendMode(blend);
	}


	void SdlGlass::setColorMod(std::optional<Color> color)
	{
		m_Pimpl->setColorMod(color);
	}


	void SdlGlass::setAlphaMod(std::optional<float> alpha)
	{
		m_Pimpl->setAlphaMod(alpha);
	}

}
