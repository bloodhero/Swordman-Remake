#pragma once

#include <string>
#include <memory>
#include "Math/Vector2.h"
#include "Renderer/Renderable.h"

namespace meow {
	class Texture
	{
	public:
		virtual void* getRawData() = 0;
		virtual Vector2i getSize() = 0;
	};

	class Image :public Texture
	{
	public:
		virtual ~Image() = default;
	};

	class Canvas :public Texture
	{
	public:
		virtual ~Canvas() = default;
		virtual void pushLayer(Renderable* ra, Vector2i pos) = 0;
	};

	class SdlImage :Image
	{
	public:
		SdlImage(std::string_view filename);
		void* getRawData() override;
		Vector2i getSize() override;

	private:
		struct Impl;
		std::unique_ptr<Impl> m_Pimpl;
	};

	class SdlCanvas :Canvas
	{
	public:
		SdlCanvas(int width, int height);
		void* getRawData() override;
		Vector2i getSize() override;

	private:
		void pushLayer(Renderable* ra, Vector2i pos) override;

	private:
		struct Impl;
		std::unique_ptr<Impl> m_Pimpl;
	};
}
