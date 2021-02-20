#include "StartScene.h"

namespace meow {
	StartScene::StartScene()
	{
		getLayerStack()->PushLayer(new TestbedLayer());
	}

	void StartScene::onNuklear()
	{
		auto context = static_cast<nk_context*>(Manager::getManager()->getNuklear()->getContext());
		struct nk_rect bounds = { 40,40,0,0 };
		bounds.w = 400;
		bounds.h = 400;
		if (nk_begin(context, "Test", bounds, NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE | NK_WINDOW_TITLE))
		{
			enum { EASY, HARD };
			static int op = EASY;
			static int property = 20;
			nk_layout_row_static(context, 30, 80, 1);
			if (nk_button_label(context, "button")) {
				OpenALMixChunk* chunk = new OpenALMixChunk("0.wav");
				Manager::getManager()->getAudio()->playSFX(chunk);

			}
			nk_layout_row_dynamic(context, 40, 2);
			if (nk_option_label(context, "easy", op == EASY)) op = EASY;
			if (nk_option_label(context, "hard", op == HARD)) op = HARD;
			nk_layout_row_dynamic(context, 45, 1);
			nk_property_int(context, "Compression:", 0, &property, 100, 10, 1);
		}
		nk_end(context);

	}

}
