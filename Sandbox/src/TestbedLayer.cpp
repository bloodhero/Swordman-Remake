#include "TestbedLayer.h"

namespace meow {


	TestbedLayer::TestbedLayer() :
		shape(100.f)
	{
		//shape.setFillColor(sf::Color::Green);
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
		Manager::getManager()->getRenderer()->draw(shape);
	}


	void TestbedLayer::onUpdate(float time)
	{

	}

}
