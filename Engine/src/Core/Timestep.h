#pragma once

#include <cstdint>


namespace meow {

	class Timestep
	{
	public:
		// CREARTORS
		Timestep();
		~Timestep() = default;

		// ACCESSORS
		void update();
		float getSeconds() const;
		float getMilliseconds() const;

	private:
		float m_Timestep;
		float m_CurrentTime;
		float m_LastFrameTime;
	};

}
