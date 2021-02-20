#pragma once

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

		SdlGlass* glass;
		SdlImage* image;
		SdlCanvas* canvas;
		Picture* picture;
		Camera* cam;
		Shared<Animation> ani;
	};
}
