#pragma once
#include <SFML/Graphics.hpp>
#include <Engine.h>

namespace meow {
	class TestbedLayer :public Layer
	{

	public:
		TestbedLayer();
		~TestbedLayer();
		void onAttach() override;


		void onDetach() override;


		void onDraw() override;


		void onUpdate(float time) override;

	private:

		sf::CircleShape shape;
	};
}
