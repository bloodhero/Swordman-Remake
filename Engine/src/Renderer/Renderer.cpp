#include "pch.h"
#include "Renderer/Renderer.h"
#include "Core/Manager.h"
#include <SDL.h>

namespace meow {
	struct Renderer::Impl {
		Impl();
		SDL_Window* windowHandler;
		sf::Vector2u getSize();
		void display();
	};

	sf::Vector2u Renderer::Impl::getSize()
	{
		int w, h;
		SDL_GetWindowSize(windowHandler, &w, &h);
		return { static_cast<unsigned int>(w), static_cast<unsigned int>(h) };
	}

	Renderer::Impl::Impl()
	{
		windowHandler = static_cast<SDL_Window*>(Manager::getManager()->getWindow()->getRawRenderer());
	}

	void Renderer::Impl::display()
	{
		SDL_GL_SwapWindow(windowHandler);
	}

	sf::Vector2u Renderer::getSize() const
	{
		return m_Pimpl->getSize();
	}

	Renderer::Renderer() :
		m_Pimpl(std::make_unique<Impl>())
	{
		// We can now initialize the render target part
		RenderTarget::initialize();
	}


	Renderer::~Renderer()
	{

	}


	bool Renderer::setActive(bool active)
	{
		return RenderTarget::setActive(active);

		// If FBOs are available, make sure none are bound when we
		// try to draw to the default framebuffer of the RenderWindow
	}


	void Renderer::display()
	{
		m_Pimpl->display();
	}

}
