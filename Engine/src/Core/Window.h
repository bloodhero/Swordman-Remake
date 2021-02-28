#pragma once

#include <memory>
#include <string>
#include <SFML/System/Vector2.hpp>
using namespace sf;
namespace meow {

	// Interface representing a desktop system based Window
	class Window
	{
	public:

		// CREARTORS
		virtual ~Window() = default;

		// ACCESSORS
		virtual bool isResizable() = 0;
		virtual bool isFullScreen() = 0;
		virtual bool isVSync() = 0;
		virtual Vector2i getResolution() = 0;

		// MANIPULATORS
		virtual void setResizable(bool resize) = 0;
		virtual void setResolution(Vector2i size) = 0;
		virtual void setFullScreen(bool full) = 0;
		virtual void setWindowTitle(std::string_view title) = 0;
		virtual void setWindowIcon(std::string_view icon_file) = 0;
		virtual void setVSync(bool enabled) = 0;
		virtual void* getRawRenderer() = 0;
	};

	class SdlWindow :public Window
	{

	public:
		SdlWindow();
		~SdlWindow() = default;
		bool isResizable() override;
		bool isFullScreen() override;
		bool isVSync() override;
		Vector2i getResolution() override;
		void setResizable(bool resize) override;
		void setResolution(Vector2i size) override;
		void setFullScreen(bool full) override;
		void setWindowTitle(std::string_view title) override;
		void setWindowIcon(std::string_view icon_file) override;
		void setVSync(bool enabled) override;
		void* getRawRenderer() override;

	private:
		struct Impl;
		std::unique_ptr<Impl> m_Pimpl;
	};
}
