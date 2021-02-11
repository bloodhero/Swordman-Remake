#pragma once

#include "Core/Utils.h"

namespace meow {
	class EventPackage;
	class Scene;
	class SceneStack;
	class Layer;
	class Application;
	class Audio;
	class Log;
	class Window;
	class GfxDevice;

	class Manager
	{
	public:
		// CREARTORS
		static Ref<Manager>& getManager() { return s_Instance; }

		// ACCESSORS
		Audio* getAudio();
		Log* getCoreLog();
		Log* getClientLog();

		// MANIPULATORS
		void runWithScene(Scene* scene);
		void replaceScene(Scene* scene);
		void pushScene(Scene* scene);
		void popScene(Scene* scene);

		void onEvent(EventPackage& e);
		void onUpdate(float time);
		void onDraw();

		void pushLayer(Layer* layer);
		void pushOverLayer(Layer* layer);
		void popLayer(Layer* Layer);
		void popOverLayer(Layer* Layer);

		void setFullScreen(bool fullScreen);
		void setResolution(int width, int height);
		void clearScreen();
		void updateScreen();

		void setAudio(Audio* a);
		void setWindow(Window* w);
		void setGfxDevice(GfxDevice* g);
		void setCoreLog(Log* l);
		void setClientLog(Log* l);

	private:
		static Ref<Manager> s_Instance;
		struct Impl;
		std::unique_ptr<Impl> m_Pimpl;
	};
}
