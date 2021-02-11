#include "pch.h"
#include "Events/Event.h"
#include "SDL.h"

namespace meow {


	meow::EventPackage EventPackage::PollEvent()
	{
		EventPackage epkg;
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			Event event = convertSdlEvent(e);
			epkg.m_Impl->m_queue.push(event);
		}

		while (!s_MsgBox.empty())
		{
			Event* event = s_MsgBox.front();
			s_MsgBox.pop();
			epkg.m_Impl->m_queue.push(event);
		}

		return epkg;
	}


	void EventPackage::PushEvent(Event* e)
	{
		s_MsgBox.push(e);
	}


	bool EventPackage::isEmpty() const
	{

	}


	meow::EventType EventPackage::getFirstType() const
	{

	}


	meow::Event* EventPackage::popEvent()
	{

	}


	meow::EventPackage EventPackage::filterEvent(EventCategory category)
	{

	}

}
