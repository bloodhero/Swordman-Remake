#include "Engine.h"
#include <nuklear.h>
#include "StartScene.h"
#include "Core/EntryPoint.h"

namespace meow {

	Scene* createStartScene()
	{
		return new StartScene();
	}
}
