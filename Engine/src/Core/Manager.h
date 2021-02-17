#pragma once

#include "Audio/Audio.h"
#include "Core/Window.h"
#include "Renderer/GfxDevice.h"
#include "Core/Log.h"
#include "Nuklear/Nuklear.h"

namespace meow {

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
