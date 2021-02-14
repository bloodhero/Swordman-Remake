#include "pch.h"

#include "SDL.h"
#include "Core/Application.h"
#include "Core/Manager.h"
#include "Core/Timestep.h"
#include "Core/Scene.h"
#include "Renderer/GfxDevice.h"
#include "Nuklear/Nuklear.h"
#include "Audio/Audio.h"
#include "Core/Window.h"
#include "Nuklear/Nuklear.h"

namespace meow {

	struct Application::Impl
	{
		void onRun();
		void onClose();
		void onEvent(SDL_Event& e);

		AppConfig* appConfig;
		bool isRunning;
		bool isMinimized;
		SceneStack* sceneStack;
	};

	void Application::Impl::onRun()
	{
		Timestep ts;
		auto manager = Manager::getManager();
		auto nuklear = manager->getNuklear();
		SDL_Event e;

		nuklear->begin();
		while (isRunning)
		{
			ts.update();

			// 处理事件
			nuklear->eventBegin();
			while (SDL_PollEvent(&e))
			{
				this->onEvent(e);
				nuklear->onEvent(&e);
			}
			nuklear->eventEnd();

			// 更新
			sceneStack->getCurrentScene()->onUpdate(ts.getMilliseconds());

			// 渲染
			manager->getGfxDevice()->clearScreen();
			sceneStack->getCurrentScene()->onDraw();
			sceneStack->getCurrentScene()->onNuklearRender();
			nuklear->render();
			manager->getGfxDevice()->updateScreen();
		}
		nuklear->end();
	}

	void Application::Impl::onClose()
	{
		isRunning = false;
	}

	void Application::Impl::onEvent(SDL_Event& event)
	{
		if (event.type == SDL_QUIT)
			onClose();
	}

	Application::Application() :
		m_Pimpl(std::make_unique<Impl>())
	{
		// 设置服务
		auto manager = Manager::getManager();
		manager->setAudio(new OpenALAudio());
		manager->setClientLog(new spdlogLog("client.log"));
		manager->setCoreLog(new spdlogLog("Engine.log"));
		manager->setWindow(new SdlWindow());
		manager->setGfxDevice(new SdlGfxDevice());
		manager->setNuklear(new SdlsurfaceNuklear());

		// 加载配置 app-config.json
		AppConfig app_cfg;

		manager->getWindow()->setWindowTitle(app_cfg.windowTitle);
		manager->getWindow()->setWindowIcon(app_cfg.windowIcon);
		manager->getGfxDevice()->setLogicalSize(app_cfg.size);
		manager->getWindow()->setFullScreen(app_cfg.isFullScreen);
		manager->getWindow()->setResizable(app_cfg.isResizable);


		// 设置日志服务
		if (app_cfg.isEnableLog)
		{
			manager->setClientLog(new spdlogLog("client.log"));
			manager->setCoreLog(new spdlogLog("Engine.log"));
		}
		else
		{
			manager->setClientLog(new NullLog());
			manager->setCoreLog(new NullLog());
		}

	}


	Application::Application():
		m_Pimpl(std::make_unique<Impl>())
	{

	}

	void Application::onRun()
	{
		m_Pimpl->onRun();
	}


	void Application::runWithScene(Scene* scene)
	{
		m_Pimpl->sceneStack->pushScene(scene);
	}


	void Application::replaceScene(Scene* scene)
	{
		m_Pimpl->sceneStack->replaceScene(scene);
	}


	void Application::pushScene(Scene* scene)
	{
		m_Pimpl->sceneStack->pushScene(scene);
	}


	void Application::popScene()
	{
		m_Pimpl->sceneStack->popScene();
	}

	std::shared_ptr<meow::Application> Application::s_Instance;

}
