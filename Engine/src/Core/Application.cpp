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
#include "Renderer/Camera.h"

namespace meow {

	struct Application::Impl
	{
		void onRun();
		void onClose();
		void onEvent(SDL_Event& e);

		bool isRunning = true;
		bool isMinimized = false;
		SceneStack* sceneStack = new SceneStack();
		~Impl();
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
			ts.onUpdate();

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
			Camera::onUpdate(ts.getMilliseconds());
			manager->getAudio()->onUpdate(ts.getMilliseconds());

			if (!isMinimized)
			{
				// 渲染
				manager->getPostEffect()->begin();
				manager->getGfxDevice()->clearScreen();
				sceneStack->getCurrentScene()->onDraw();
				manager->getPostEffect()->end();

				// UI不做后期处理
				sceneStack->getCurrentScene()->onNuklear();
				nuklear->render();
				manager->getGfxDevice()->updateScreen();
			}
		}
		nuklear->end();
	}

	void Application::Impl::onClose()
	{
		isRunning = false;
	}

	void Application::Impl::onEvent(SDL_Event& event)
	{
		switch (event.type)
		{
		case SDL_WINDOWEVENT:
			switch (event.window.event)
			{
			case SDL_WINDOWEVENT_MINIMIZED:
				isMinimized = true;
				break;

			case SDL_WINDOWEVENT_RESTORED:
				isMinimized = false;
				break;
			}

			break;

		case SDL_QUIT:
			onClose();
			break;

		}
	}

	Application::Impl::~Impl()
	{
		delete sceneStack;
	}

	Application::Application() :
		m_Pimpl(std::make_unique<Impl>())
	{

		auto manager = Manager::getManager();
		AppConfig app_cfg;

		// 优先设置日志服务
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

		manager->setAudio(new OpenALAudio());
		manager->setWindow(new SdlWindow());
		manager->setGfxDevice(new SdlGfxDevice());
		manager->setNuklear(new SdlsurfaceNuklear());
		manager->setPostEffect(new NullPostEffect());

		manager->getWindow()->setWindowTitle(app_cfg.windowTitle);
		manager->getWindow()->setWindowIcon(app_cfg.windowIcon);
		manager->getWindow()->setResolution(app_cfg.resolution);
		manager->getGfxDevice()->setLogicalSize(app_cfg.logicalSize);
		manager->getWindow()->setFullScreen(app_cfg.isFullScreen);
		manager->getWindow()->setResizable(app_cfg.isResizable);

	}


	Application* Application::getApplication()
	{
		static Application instance;
		return &instance;
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


	meow::Scene* Application::getCurrentScene()
	{
		return m_Pimpl->sceneStack->getCurrentScene();
	}

}
