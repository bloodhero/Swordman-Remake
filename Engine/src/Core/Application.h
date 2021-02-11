#pragma once


// CREARTORS

// ACCESSORS

// MANIPULATORS


#include <string>

#include "Core/Utils.h"

namespace meow {
	class Scene;

	struct AppInitConfig
	{
		std::string windowTitle;
		std::string windowIcon;
		std::string resourceDir;
		int width;
		int height;
		bool isFullScreen;
		bool isResizable;
	};

	class Application
	{
	public:
		// CREARTORS
		~Application() = default;
		static Ref<Application>& getApplication() { return s_Instance; }

		// ACCESSORS

		// MANIPULATORS
		void onRun(); // 启动游戏主循环
		void onConfig(Bootstrap* booter); // 配置各个子系统

	private:
		Application() :p_Impl(CreateScope<Impl>()) {};

		struct Impl;
		Scope<Impl> p_Impl;

		static Ref<Application> s_Instance;
	};

	class Bootstrap
	{
	public:
		// CREARTORS
		virtual ~Bootstrap() = default;

		// ACCESSORS

		// MANIPULATORS
		virtual void configApp(AppInitConfig* config) = 0;
		virtual Scene* firstScene() = 0;
	};

	// To be defined in CLIENT
	extern meow::Bootstrap* meow::createBootstrap();
}
