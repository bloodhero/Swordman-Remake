#include "pch.h"

#include "Core/Application.h"
#include "Core/Manager.h"
#include "Core/Timestep.h"
#include "Events/Event.h"


namespace meow {


	struct Application::Impl
	{
		void onRun();
		void onClose();
		void onEvent(EventPackage& e);

		AppInitConfig* appConfig;
		bool isRunning;
		bool isMinimized;
	};

	void Application::Impl::onRun()
	{
		Timestep ts;
		auto manager = Manager::getManager();
		while (isRunning)
		{
			ts.update();
			EventPackage event_pkg = EventPackage::pollEvent();
			onEvent(event_pkg);

			if (!event_pkg.isEmpty())
				manager->onEvent(event_pkg);

			manager->clearScreen();
			manager->onUpdate(ts.getMilliseconds());
			manager->onDraw();
			manager->updateScreen();

		}

	}

	void Application::Impl::onClose()
	{
		isRunning = false;
	}

	void Application::Impl::onEvent(EventPackage& pkg)
	{
		EventPackage p = pkg.filterEvent(EventCategoryApplication);



		if (p.hasEvent())
		{
			isRunning = false;
			pkg.consumeEvent();
		}
	}

	void Application::onRun()
	{
		p_Impl->onRun();
	}


	void Application::onConfig(Bootstrap* booter)
	{

	}


	meow::Ref<meow::Application> Application::s_Instance;

}
