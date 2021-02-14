#include "Engine.h"

namespace meow {

	class StartScene :public Scene
	{
	public:
		StartScene() = default;
		~StartScene() = default;
		void onPause() override
		{

		}


		void onResume() override
		{

		}


		void onNuklear() override
		{

		}

	};

	Scene* createStartScene()
	{
		return new StartScene();
	}
}
