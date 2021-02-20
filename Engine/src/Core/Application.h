#pragma once


// CREARTORS

// ACCESSORS

// MANIPULATORS


#include <string>
#include "Math/Vector2.h"
#include "Core/Utils.h"

namespace meow {
	class Scene;

	struct AppConfig
	{
		std::string windowTitle = "Swordman";
		std::string windowIcon = "";
		std::string resourceDir = "";
		Vector2i logicalSize = { 400, 240};
		Vector2i resolution = { 800, 480 };
		bool isFullScreen = false;
		bool isResizable = true;
		bool isEnableLog = true;
	};

	class Application
	{
	public:
		// CREARTORS
		~Application() = default;
		static Application* getApplication();

		// ACCESSORS

		// MANIPULATORS
		void onRun(); // 启动游戏主循环
		void runWithScene(Scene* scene);
		void replaceScene(Scene* scene);
		void pushScene(Scene* scene);
		void popScene();
		Scene* getCurrentScene();

	private:
		Application();

		struct Impl;
		std::unique_ptr<Impl> m_Pimpl;
	};

	// To be defined in CLIENT
	extern Scene* createStartScene();
}
