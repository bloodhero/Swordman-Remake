#pragma  once
#include <Engine.h>
#include <nuklear.h>

#include "TestbedLayer.h"
namespace meow {

	class StartScene :public Scene
	{
	public:
		StartScene();
		~StartScene() = default;
		void onPause() override
		{

		}


		void onResume() override
		{

		}


		void onNuklear() override;

	};



}
