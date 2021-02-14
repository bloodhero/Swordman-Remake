#pragma once


// CREARTORS

// ACCESSORS

// MANIPULATORS


#include <string>

#include "Core/Utils.h"

namespace meow {
	class Scene;

	struct AppConfig
	{
		std::string windowTitle;
		std::string windowIcon;
		std::string resourceDir;
		Vector2i size;
		bool isFullScreen;
		bool isResizable;
		bool isEnableLog;
	};

	class Application
	{
	public:
		// CREARTORS
		~Application() = default;
		static std::shared_ptr<Application>& getApplication() { return s_Instance; }

		// ACCESSORS

		// MANIPULATORS
		void onRun(); // 启动游戏主循环
		void runWithScene(Scene* scene);
		void replaceScene(Scene* scene);
		void pushScene(Scene* scene);
		void popScene();

	private:
		Application();

		struct Impl;
		std::unique_ptr<Impl> m_Pimpl;
		static std::shared_ptr<Application> s_Instance;
	};

	// To be defined in CLIENT
	extern Scene* createStartScene();
}
