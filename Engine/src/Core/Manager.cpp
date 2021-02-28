#include "pch.h"
#include "Audio/Audio.h"
#include "Core/Manager.h"
#include "Core/Log.h"
#include "Core/Window.h"
#include "Nuklear/Nuklear.h"
#include "Application.h"
#include "Core/Layer.h"

namespace meow {
	struct Manager::Impl
	{
		Audio* audio = nullptr;
		Window* window = nullptr;
		Log* coreLog = nullptr;
		Log* clientLog = nullptr;
		Nuklear* nuklear = nullptr;
		Renderer* renderer = nullptr;
		~Impl();
	};

	Manager::Impl::~Impl()
	{
		delete audio;
		delete window;
		delete coreLog;
		delete clientLog;
		delete nuklear;
	}


	Manager* Manager::getManager()
	{
		static Manager instance;
		return &instance;
	}

	meow::Audio* Manager::getAudio()
	{
		return m_Pimpl->audio;
	}

	meow::Window* Manager::getWindow()
	{
		return m_Pimpl->window;
	}

	meow::Log* Manager::getCoreLog()
	{
		return m_Pimpl->coreLog;
	}


	meow::Log* Manager::getClientLog()
	{
		return m_Pimpl->clientLog;
	}


	meow::Nuklear* Manager::getNuklear()
	{
		return m_Pimpl->nuklear;
	}


	meow::Renderer* Manager::getRenderer()
	{
		return m_Pimpl->renderer;
	}

	void Manager::setAudio(Audio* a)
	{
		if (m_Pimpl->audio) delete m_Pimpl->audio;
		m_Pimpl->audio = a;
	}


	void Manager::setWindow(Window* w)
	{
		if (m_Pimpl->window) delete m_Pimpl->window;
		m_Pimpl->window = w;
	}

	void Manager::setCoreLog(Log* l)
	{
		if (m_Pimpl->coreLog) delete m_Pimpl->coreLog;
		m_Pimpl->coreLog = l;
	}


	void Manager::setClientLog(Log* l)
	{
		if (m_Pimpl->clientLog) delete m_Pimpl->clientLog;
		m_Pimpl->clientLog = l;
	}


	void Manager::setNuklear(Nuklear* n)
	{
		if (m_Pimpl->nuklear) delete m_Pimpl->nuklear;
		m_Pimpl->nuklear = n;
	}


	void Manager::setRenderer(Renderer* r)
	{
		if (m_Pimpl->renderer) delete m_Pimpl->renderer;
		m_Pimpl->renderer = r;
	}

	void Manager::pushLayer(Layer* l)
	{
		Application::getApplication()->getCurrentScene()->getLayerStack()->PushLayer(l);
	}


	void Manager::pushOverlay(Layer* l)
	{
		Application::getApplication()->getCurrentScene()->getLayerStack()->PushOverlay(l);
	}


	void Manager::popLayer(Layer* l)
	{
		Application::getApplication()->getCurrentScene()->getLayerStack()->PopLayer(l);
	}


	void Manager::popOverlay(Layer* l)
	{
		Application::getApplication()->getCurrentScene()->getLayerStack()->PopOverlay(l);
	}


	void Manager::runWithScene(Scene* s)
	{
		Application::getApplication()->runWithScene(s);
	}


	void Manager::replaceScene(Scene* s)
	{
		Application::getApplication()->replaceScene(s);
	}


	void Manager::pushScene(Scene* s)
	{
		Application::getApplication()->pushScene(s);
	}


	void Manager::popScene()
	{
		Application::getApplication()->popScene();
	}

	Manager::Manager()
		:m_Pimpl(std::make_unique<Impl>())
	{

	}

}
