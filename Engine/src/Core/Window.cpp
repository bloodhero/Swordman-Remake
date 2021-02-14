#include <SDL.h>
#include <memory>
#include "Core/Window.h"
#include "Core/Utils.h"
#include "Renderer/Texture.h"

namespace meow {
	struct SdlWindow::Impl {
		Vector2i resolution = { 800, 600 };
		bool isFullScreen = false;
		bool isResizable = true;
		bool isVsync = true;
		std::string windowTitle = "Swordman Engine";
		std::string windowIcon = "";
		SDL_Window* windowHandle = nullptr;
		SDL_Renderer* rendererHandle = nullptr;

		void setResizable(bool resize);
		void setFullScreen(bool full);
		void setVSync(bool vsync);
		void setWindowTitle(std::string_view title);
		void setWindowIcon(std::string_view icon_file);
		void setResolution(Vector2i size);
		Impl();
		~Impl();
	};

	void SdlWindow::Impl::setResizable(bool resize)
	{
		isResizable = resize;
		SDL_SetWindowResizable(windowHandle, resize ? SDL_TRUE : SDL_FALSE);

		LOGGER->trace("Window resizable is {}", resize ? "enabled" : "disabled");
	}

	void SdlWindow::Impl::setFullScreen(bool full)
	{
		isFullScreen = full;
		SDL_SetWindowFullscreen(windowHandle, full ? SDL_WINDOW_FULLSCREEN : 0);
		if (!full) SDL_SetWindowPosition(windowHandle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

		LOGGER->trace("Window fullscreen is {}", full ? "enabled" : "disabled");
	}

	void SdlWindow::Impl::setVSync(bool vsync)
	{
		SDL_SetHint(SDL_HINT_RENDER_VSYNC, vsync ? "1" : "0");
	}

	void SdlWindow::Impl::setWindowTitle(std::string_view title)
	{
		windowTitle = title;
		SDL_SetWindowTitle(windowHandle, title.data());

		LOGGER->trace("Window title : {}", title.data());
	}

	void SdlWindow::Impl::setWindowIcon(std::string_view icon_file)
	{

		windowIcon = icon_file;
		SDL_Surface* surface = SDL_LoadBMP(icon_file.data());
		SDL_SetWindowIcon(windowHandle, surface);
		SDL_FreeSurface(surface);

		LOGGER->trace("Window icon file is {}", icon_file.data());

	}

	void SdlWindow::Impl::setResolution(Vector2i size)
	{
		ASSERT(size.x > 0 && size.y > 0, "Window resolution can't be {}x{}", size.x, size.y);

		resolution = size;
		SDL_SetWindowSize(windowHandle, resolution.x, resolution.y);
		SDL_SetWindowPosition(windowHandle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

		LOGGER->trace("Window resized to {}x{}", size.x, size.y);
	}

	SdlWindow::Impl::Impl()
	{
		int result = SDL_Init(SDL_INIT_VIDEO);
		ASSERT(!result, "SDL: Failed to initialize the SDL system. {}", SDL_GetError());

		windowHandle = SDL_CreateWindow(windowTitle.c_str(),
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			resolution.x,
			resolution.y,
			SDL_WINDOW_SHOWN );

		ASSERT(windowHandle, "SDL: Failed to initialize the SDL window. {}", SDL_GetError());

		// create renderer context
		rendererHandle = SDL_CreateRenderer(windowHandle, -1, SDL_RENDERER_ACCELERATED);


		LOGGER->trace("SDL initialized.");

		// ≈‰÷√¥∞ø⁄ Ù–‘
		setVSync(isVsync);
		setResolution(resolution);
		setFullScreen(isFullScreen);
		setResizable(isResizable);
		setWindowTitle(windowTitle);
	}

	SdlWindow::Impl::~Impl()
	{
		SDL_DestroyRenderer(rendererHandle);
		SDL_DestroyWindow(windowHandle);
		SDL_Quit();
	}

	SdlWindow::SdlWindow() :
		m_Pimpl(std::make_unique<Impl>())
	{
	}

	bool SdlWindow::isResizable()
	{
		return m_Pimpl->isResizable;
	}

	bool SdlWindow::isFullScreen()
	{
		return m_Pimpl->isFullScreen;
	}


	bool SdlWindow::isVSync()
	{
		return m_Pimpl->isVsync;
	}


	meow::Vector2i SdlWindow::getResolution()
	{
		return m_Pimpl->resolution;
	}


	void SdlWindow::setResizable(bool resize)
	{
		m_Pimpl->setResizable(resize);
	}


	void SdlWindow::setResolution(Vector2i size)
	{
		m_Pimpl->setResolution(size);
	}


	void SdlWindow::setFullScreen(bool full)
	{
		m_Pimpl->setFullScreen(full);
	}


	void SdlWindow::setVSync(bool vsync)
	{
		m_Pimpl->setVSync(vsync);
	}


	void* SdlWindow::getRawRenderer()
	{
		return m_Pimpl->rendererHandle;
	}

	void SdlWindow::setWindowTitle(std::string_view title)
	{
		m_Pimpl->setWindowTitle(title);
	}


	void SdlWindow::setWindowIcon(std::string_view icon_file)
	{
		m_Pimpl->setWindowIcon(icon_file);
	}

}
