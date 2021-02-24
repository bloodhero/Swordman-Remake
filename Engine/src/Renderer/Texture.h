#pragma once

#include <string>
#include <memory>
#include "Math/Vector2.h"
#include "Renderer/Renderable.h"

namespace meow {
	class Texture
	{
	public:
		virtual ~Texture() = default;
		virtual void* getRawData() = 0;
		virtual Vector2i getSize() = 0;
		virtual void draw(std::optional<Rect> src, std::optional<Rect> dst) = 0;
		virtual void setBlendMode(std::optional<Renderable::BlendMode> blend) = 0;
		virtual void setColorMod(std::optional<Color> color) = 0;
		virtual void setAlphaMod(std::optional<float> alpha) = 0;
	};


	// 图片
	class Image :public Texture
	{
	public:
		virtual ~Image() = default;
	};


	// 画布
	class Canvas :public Texture
	{
	public:
		virtual ~Canvas() = default;
		virtual void pushLayer(Renderable ra, Vector2i pos) = 0;
	};


	// 生成半透明的窗口大小的颜色蒙版
	class Glass :public Texture
	{
	public:
		virtual ~Glass() = default;

	};


	class SdlImage :public Image
	{
	public:
		SdlImage(std::string_view filename);
		void* getRawData() override;
		Vector2i getSize() override;
		void draw(std::optional<Rect> src, std::optional<Rect> dst) override;
		void setBlendMode(std::optional<Renderable::BlendMode> blend) override;
		void setColorMod(std::optional<Color> color) override;
		void setAlphaMod(std::optional<float> alpha) override;

	private:
		struct Impl;
		std::unique_ptr<Impl> m_Pimpl;
	};


	class SdlCanvas :public Canvas
	{
	public:
		SdlCanvas(int width, int height);
		void* getRawData() override;
		Vector2i getSize() override;
		void pushLayer(Renderable ra, Vector2i pos) override;
		void draw(std::optional<Rect> src, std::optional<Rect> dst) override;
		void setBlendMode(std::optional<Renderable::BlendMode> blend) override;
		void setColorMod(std::optional<Color> color) override;
		void setAlphaMod(std::optional<float> alpha) override;

	private:
		struct Impl;
		std::unique_ptr<Impl> m_Pimpl;
	};


	class SdlGlass :public Glass
	{
	public:
		SdlGlass(Color c);
		~SdlGlass();
		void* getRawData() override;
		Vector2i getSize() override;
		void draw(std::optional<Rect> src, std::optional<Rect> dst) override;
		void setBlendMode(std::optional<Renderable::BlendMode> blend) override;
		void setColorMod(std::optional<Color> color) override;
		void setAlphaMod(std::optional<float> alpha) override;

	private:
		struct Impl;
		std::unique_ptr<Impl> m_Pimpl;
	};

}
