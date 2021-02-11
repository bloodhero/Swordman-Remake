#pragma once

#include "Core/Command.h"

namespace meow {
	class InputHandler
	{
	public:
		virtual Command* handleInput() = 0;
	};


}
