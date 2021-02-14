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
	class Nuklear;

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

		void setAudio(Audio* a);
		void setWindow(Window* w);
		void setGfxDevice(GfxDevice* g);
		void setCoreLog(Log* l);
		void setClientLog(Log* l);
		void setNuklear(Nuklear* n);

	private:
		Manager();

		static Manager* s_Instance;
		struct Impl;
		std::unique_ptr<Impl> m_Pimpl;
	};
}
