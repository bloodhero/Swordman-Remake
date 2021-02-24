#pragma once

namespace meow {
	class Component
	{

	};

	class System
	{
	public:
		virtual void Update(float timestep) = 0;

	};
}
