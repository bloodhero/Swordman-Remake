#include "pch.h"
#include "Renderer/PostEffect.h"
#include <SDL.h>
#include "Core/Utils.h"

namespace meow {

	struct PostEffect::Impl
	{
		Impl();
		~Impl() = default;

		Vector2i size;
		Shared<SdlCanvas> canvas;
		Renderable ra;
		void restore();
		void begin();
		void end();
		void setSlice(std::optional<Rect> s);
		void setAlphaMod(std::optional<float> a);
		void setColorMod(std::optional<Color> c);
	};

	void PostEffect::Impl::setSlice(std::optional<Rect> s)
	{
		ra.slice = s;
	}

	void PostEffect::Impl::setAlphaMod(std::optional<float> a)
	{
		ra.alphaMod = a;
	}

	void PostEffect::Impl::setColorMod(std::optional<Color> c)
	{
		ra.colorMod = c;
	}

	void PostEffect::Impl::restore()
	{
		ra.slice = std::nullopt;
		ra.alphaMod = std::nullopt;
		ra.colorMod = std::nullopt;
	}

	void PostEffect::Impl::begin()
	{
		Manager::getManager()->getGfxDevice()->setRenderTarget(canvas.get());
	}

	void PostEffect::Impl::end()
	{
		auto gfx = Manager::getManager()->getGfxDevice();
		gfx->updateScreen();
		gfx->setRenderTarget(nullptr);
		gfx->clearScreen();

		ra.draw(std::nullopt);
	}


	PostEffect::Impl::Impl() :
		size(Manager::getManager()->getGfxDevice()->getLogicalSize()),
		canvas(CreateShared<SdlCanvas>(size.x, size.y))
	{
		ra.texture = canvas;
	}


	PostEffect::PostEffect() :
		m_Pimpl(std::make_unique<Impl>())
	{

	}

	PostEffect::~PostEffect()
	{

	}

	void PostEffect::setSlice(std::optional<Rect> s)
	{
		m_Pimpl->setSlice(s);
	}


	void PostEffect::setAlphaMod(std::optional<float> a)
	{
		m_Pimpl->setAlphaMod(a);
	}


	void PostEffect::setColorMod(std::optional<Color> c)
	{
		m_Pimpl->setColorMod(c);
	}


	void PostEffect::restore()
	{
		m_Pimpl->restore();
	}


	void PostEffect::begin()
	{
		m_Pimpl->begin();
	}


	void PostEffect::end()
	{
		m_Pimpl->end();
	}

}
