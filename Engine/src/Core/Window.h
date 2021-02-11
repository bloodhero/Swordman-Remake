#pragma once

#include <memory>
#include <string>
#include "Math/Vector2.h"

namespace meow {

	class Texture;

	// Interface representing a desktop system based Window
	class Window
	{
	public:

		// CREARTORS
		~Window() = default;

		// ACCESSORS
		bool isResizable();
		bool isFullScreen();
		bool isVSync();
		Vector2i getResolution();

		// MANIPULATORS
		void setResizable(bool resize);
		void setResolution(Vector2i size);
		void setFullScreen(bool full);
		void setWindowTitle(std::string_view title);
		void setWindowIcon(Texture* tex);
		void setVSync(bool enabled);

	private:
		struct Impl;
		std::unique_ptr<Impl> m_Pimpl;
	};

	class SdlWindow :public Window
	{

	};
}
