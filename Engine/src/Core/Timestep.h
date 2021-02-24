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
		void onUpdate();
		double getSeconds() const;
		double getMilliseconds() const;

	private:
		double m_Timestep;
		double m_CurrentTime;
		double m_LastFrameTime;
	};

}
