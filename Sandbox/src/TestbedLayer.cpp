#include "TestbedLayer.h"

namespace meow {


	TestbedLayer::TestbedLayer() :
		cam(new Camera()),
		picture(nullptr)
	{
		ani = CreateShared<Animation>("data/man_eff/Animation.json", cam);
		LOGGER->info("dirname is {}", dirName("data/Animation.json"));
	}


	TestbedLayer::~TestbedLayer()
	{

	}

	void TestbedLayer::onAttach()
	{

	}

	void TestbedLayer::onDetach()
	{

	}


	void TestbedLayer::onDraw()
	{
		ani->onDraw();
	}


	void TestbedLayer::onUpdate(float time)
	{
		ani->onUpdate(time);
	}

}
