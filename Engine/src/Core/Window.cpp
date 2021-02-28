#include "pch.h"
#include <SDL.h>
#include "Core/Window.h"
#include "Core/Utils.h"
#include <glad/glad.h>

namespace meow {
	struct SdlWindow::Impl {
		Vector2i resolution = { 800, 600 };
		bool isFullScreen = false;
		bool isResizable = true;
		bool isVsync = true;
		std::string windowTitle = "Swordman Engine";
		std::string windowIcon = "";
		SDL_Window* windowHandle = nullptr;
		SDL_GLContext glContext;

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

		LOGGER->trace("{}: Window resizable is {}", __func__, resize ? "enabled" : "disabled");
	}

	void SdlWindow::Impl::setFullScreen(bool full)
	{
		isFullScreen = full;
		SDL_SetWindowFullscreen(windowHandle, full ? SDL_WINDOW_FULLSCREEN : 0);
		if (!full) SDL_SetWindowPosition(windowHandle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

		LOGGER->trace("{}: Window fullscreen is {}", __func__, full ? "enabled" : "disabled");
	}

	void SdlWindow::Impl::setVSync(bool vsync)
	{
		isVsync = vsync;
		SDL_GL_SetSwapInterval(vsync ? 1 : 0);
	}

	void SdlWindow::Impl::setWindowTitle(std::string_view title)
	{
		if (!title.empty())
		{
			windowTitle = title;
			SDL_SetWindowTitle(windowHandle, title.data());

			LOGGER->trace("Window title : {}", windowTitle.c_str());
		}
	}

	void SdlWindow::Impl::setWindowIcon(std::string_view icon_file)
	{
		if (!icon_file.empty())
		{
			windowIcon = icon_file;
			SDL_Surface* surface = SDL_LoadBMP(icon_file.data());
			SDL_SetWindowIcon(windowHandle, surface);
			SDL_FreeSurface(surface);

			LOGGER->trace("Window icon file is {}", windowIcon.c_str());
		}
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

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		windowHandle = SDL_CreateWindow(windowTitle.c_str(),
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			resolution.x,
			resolution.y,
			SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

		ASSERT(windowHandle, "SDL: Failed to initialize the SDL window. {}", SDL_GetError());

		glContext = SDL_GL_CreateContext(windowHandle);
		SDL_GL_MakeCurrent(windowHandle, glContext);

		gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);

		LOGGER->trace("SDL && OpenGL initialized.");

		// ≈‰÷√¥∞ø⁄ Ù–‘
		setVSync(isVsync);
		setResolution(resolution);
		setFullScreen(isFullScreen);
		setResizable(isResizable);
		setWindowTitle(windowTitle);
	}

	SdlWindow::Impl::~Impl()
	{
		SDL_GL_DeleteContext(glContext);
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


	Vector2i SdlWindow::getResolution()
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
		return m_Pimpl->windowHandle;
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
