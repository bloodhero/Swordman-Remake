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
		case meow::Renderable::BlendMode::NONE:
			return SDL_BLENDMODE_NONE;
			break;
		case meow::Renderable::BlendMode::BLEND:
			return SDL_BLENDMODE_BLEND;
			break;
		case meow::Renderable::BlendMode::ADD:
			return SDL_BLENDMODE_ADD;
			break;
		case meow::Renderable::BlendMode::MOD:
			return SDL_BLENDMODE_MOD;
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


	static void draw(SDL_Texture* tex, Rect src, Rect dst)
	{
		SDL_RenderCopy(getSdlRenderer(), tex, &rect2SDL_Rect(src), &rect2SDL_Rect(dst));
	}


	static void setBlendMode(SDL_Texture* tex, Renderable::BlendMode blend)
	{
		SDL_SetTextureBlendMode(tex, convertBlendMode(blend));
	}


	static void setColorMod(SDL_Texture* tex, Color color)
	{
		SDL_SetTextureColorMod(tex, color.r, color.g, color.b);
	}


	static void setAlphaMod(SDL_Texture* tex, float alpha)
	{
		SDL_SetTextureAlphaMod(tex, static_cast<Uint8>(alpha * 255));
	}


	struct SdlImage::Impl
	{
		Impl(std::string_view filename);
		~Impl();
		SDL_Texture* texture = nullptr;
		Vector2i size;
		std::string filename;
		void draw(Rect src, Rect dst) { meow::draw(texture, src, dst); }
		void setBlendMode(Renderable::BlendMode blend) { meow::setBlendMode(texture, blend); }
		void setColorMod(Color color) { meow::setColorMod(texture, color); }
		void setAlphaMod(float alpha) { meow::setAlphaMod(texture, alpha); }
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


	void SdlImage::draw(Rect src, Rect dst)
	{
		m_Pimpl->draw(src, dst);
	}


	void SdlImage::setBlendMode(Renderable::BlendMode blend)
	{
		m_Pimpl->setBlendMode(blend);
	}


	void SdlImage::setColorMod(Color color)
	{
		m_Pimpl->setColorMod(color);
	}


	void SdlImage::setAlphaMod(float alpha)
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
		void draw(Rect src, Rect dst) { meow::draw(texture, src, dst); }
		void setBlendMode(Renderable::BlendMode blend) { meow::setBlendMode(texture, blend); }
		void setColorMod(Color color) { meow::setColorMod(texture, color); }
		void setAlphaMod(float alpha) { meow::setAlphaMod(texture, alpha); }
	};


	SdlCanvas::Impl::~Impl()
	{
		SDL_DestroyTexture(texture);
	}


	void SdlCanvas::Impl::pushLayer(Renderable ra, Vector2i pos)
	{
		auto temp = SDL_GetRenderTarget(getSdlRenderer());
		SDL_SetRenderTarget(getSdlRenderer(), texture);
		SDL_Texture* sdl_tex = static_cast<SDL_Texture*>(ra.texture->getRawData());

		if (ra.alphaMod.has_value())
			ra.texture->setAlphaMod(ra.alphaMod.value());
		if (ra.colorMod.has_value())
			ra.texture->setColorMod(ra.colorMod.value());
		if (ra.blendMode.has_value())
			ra.texture->setBlendMode(ra.blendMode.value());

		Rect src_rect, dst_rect;
		Vector2i size = ra.texture->getSize();
		if (ra.slice.has_value())
			src_rect = ra.slice.value();
		else
			src_rect = { 0, 0, size.x, size.y };

		dst_rect = { pos.x, pos.y, src_rect.w, src_rect.h };
		ra.texture->draw(src_rect, dst_rect);

		Manager::getManager()->getGfxDevice()->updateScreen();

		// reset state
		ra.texture->setAlphaMod(1.0f);
		ra.texture->setBlendMode(Renderable::BlendMode::BLEND);
		ra.texture->setColorMod({ 255,255,255,255 });

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


	void SdlCanvas::draw(Rect src, Rect dst)
	{
		m_Pimpl->draw(src, dst);
	}


	void SdlCanvas::setBlendMode(Renderable::BlendMode blend)
	{
		m_Pimpl->setBlendMode(blend);
	}


	void SdlCanvas::setColorMod(Color color)
	{
		m_Pimpl->setColorMod(color);
	}


	void SdlCanvas::setAlphaMod(float alpha)
	{
		m_Pimpl->setAlphaMod(alpha);
	}


	struct SdlGlass::Impl
	{
		Impl(Color c);
		~Impl();
		SDL_Texture* texture;
		Vector2i size;
		void draw(Rect src, Rect dst) { meow::draw(texture, src, dst); }
		void setBlendMode(Renderable::BlendMode blend) { meow::setBlendMode(texture, blend); }
		void setColorMod(Color color) { meow::setColorMod(texture, color); }
		void setAlphaMod(float alpha) { meow::setAlphaMod(texture, alpha); }
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


	void SdlGlass::draw(Rect src, Rect dst)
	{
		m_Pimpl->draw(src, dst);
	}


	void SdlGlass::setBlendMode(Renderable::BlendMode blend)
	{
		m_Pimpl->setBlendMode(blend);
	}


	void SdlGlass::setColorMod(Color color)
	{
		m_Pimpl->setColorMod(color);
	}


	void SdlGlass::setAlphaMod(float alpha)
	{
		m_Pimpl->setAlphaMod(alpha);
	}

}
