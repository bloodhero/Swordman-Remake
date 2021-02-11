#pragma once

#include <memory>
#include "Math/Vector2.h"
#include "Renderer/Drawable.h"

namespace meow {

	class GfxDevice
	{
	public:
		static GfxDevice* getGfxDevice();
		static void setGfxDevice(GfxDevice* g);
		virtual void setClearColor(Color color) = 0;
		virtual void setLogicalSize(Vector2i size) = 0;

		virtual Color getClearColor() = 0;
		virtual Vector2i getLogicalSize() = 0;

		virtual void clearScreen() = 0;
		virtual void updateScreen() = 0;

	private:
		static GfxDevice* s_Instance;
	};

	class SdlGfxDevice :public GfxDevice
	{

	};
}
