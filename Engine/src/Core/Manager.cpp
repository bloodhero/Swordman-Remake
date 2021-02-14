#include "pch.h"
#include "Core/Manager.h"

namespace meow {
	struct Manager::Impl
	{
		Audio* audio = nullptr;
		Window* window = nullptr;
		GfxDevice* gfxDevice = nullptr;
		Log* coreLog = nullptr;
		Log* clientLog = nullptr;
		Nuklear* nuklear = nullptr;
		~Impl();
	};

	Manager::Impl::~Impl()
	{
		delete audio;
		delete window;
		delete gfxDevice;
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


	meow::GfxDevice* Manager::getGfxDevice()
	{
		return m_Pimpl->gfxDevice;
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


	void Manager::setAudio(Audio* a)
	{
		m_Pimpl->audio = a;
	}


	void Manager::setWindow(Window* w)
	{
		m_Pimpl->window = w;
	}


	void Manager::setGfxDevice(GfxDevice* g)
	{
		m_Pimpl->gfxDevice = g;
	}


	void Manager::setCoreLog(Log* l)
	{
		m_Pimpl->coreLog = l;
	}


	void Manager::setClientLog(Log* l)
	{
		m_Pimpl->clientLog = l;
	}


	void Manager::setNuklear(Nuklear* n)
	{
		m_Pimpl->nuklear = n;
	}


	Manager::Manager()
		:m_Pimpl(std::make_unique<Impl>())
	{

	}

}
