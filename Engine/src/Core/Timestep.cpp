#include "pch.h"
#include <SDL.h>
#include "Core/Timestep.h"

namespace meow {


	Timestep::Timestep()
		:m_CurrentTime(SDL_GetTicks()),
		m_Timestep(0),
		m_LastFrameTime(m_CurrentTime)
	{

	}

	void Timestep::onUpdate()
	{
		m_CurrentTime = SDL_GetTicks();
		m_Timestep = m_CurrentTime - m_LastFrameTime;
		m_LastFrameTime = m_CurrentTime;
	}


	double Timestep::getSeconds() const
	{
		return m_Timestep / 1000.0f;
	}


	double Timestep::getMilliseconds() const
	{
		return m_Timestep;
	}

}
