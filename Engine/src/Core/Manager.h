#pragma once

#include "Audio/Audio.h"
#include "Core/Window.h"
#include "Renderer/GfxDevice.h"
#include "Core/Log.h"
#include "Nuklear/Nuklear.h"
#include "Renderer/PostEffect.h"
#include "Core/Scene.h"

namespace meow {
	class Layer;

	class Manager
	{
	public:
		// CREARTORS
		~Manager() = default;
		static Manager* getManager();

		// ACCESSORS
		Audio* getAudio();
		Window* getWindow();
		GfxDevice* getGfxDevice();
		Log* getCoreLog();
		Log* getClientLog();
		Nuklear* getNuklear();
		PostEffect* getPostEffect();

		void setAudio(Audio* a);
		void setWindow(Window* w);
		void setGfxDevice(GfxDevice* g);
		void setCoreLog(Log* l);
		void setClientLog(Log* l);
		void setNuklear(Nuklear* n);
		void setPostEffect(PostEffect* p);

		void pushLayer(Layer* l);
		void pushOverlay(Layer* l);
		void popLayer(Layer* l);
		void popOverlay(Layer* l);

		void runWithScene(Scene* s);
		void replaceScene(Scene* s);
		void pushScene(Scene* s);
		void popScene();

	private:
		Manager();

		struct Impl;
		std::unique_ptr<Impl> m_Pimpl;
	};
}
