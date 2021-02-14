#pragma once

#include <memory>
#include "Math/Vector2.h"
#include "Renderer/Drawable.h"

namespace meow {

	class GfxDevice
	{
	public:
		enum class ScaleQuality
		{
			NEAREST,
			LINEAR,
			BEST
		};

		virtual ~GfxDevice() = default;
		virtual void setScaleQuality(ScaleQuality qlt) = 0;
		virtual void setClearColor(Color color) = 0;
		virtual void setLogicalSize(Vector2i size) = 0;

		virtual Color getClearColor() = 0;
		virtual Vector2i getLogicalSize() = 0;

		virtual void clearScreen() = 0;
		virtual void updateScreen() = 0;
	};

	class SdlGfxDevice :public GfxDevice
	{

	public:
		void setClearColor(Color color) override;
		void setLogicalSize(Vector2i size) override;
		Color getClearColor() override;
		Vector2i getLogicalSize() override;
		void clearScreen() override;
		void updateScreen() override;
		void setScaleQuality(ScaleQuality qlt) override;

	};
}
