#include "Engine.h"

namespace meow {

	class StartScene :public Scene
	{
	public:
		StartScene();
		~StartScene();
		void onPause() override
		{

		}


		void onResume() override
		{

		}


		void onNuklearRender() override
		{
		}

	};

	Scene* createStartScene()
	{
		return new StartScene();
	}
}
