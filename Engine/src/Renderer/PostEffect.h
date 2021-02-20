#pragma once
#include "Renderer/Renderable.h"
#include "Renderer/Texture.h"

namespace meow {
	class PostEffect
	{
	public:
		virtual ~PostEffect() = default;
		virtual void setSlice(Rect s) = 0;
		virtual void setAlphaMod(float a) = 0;
		virtual void setColorMod(Color c) = 0;
		virtual void restore() = 0;
		virtual void begin() = 0;
		virtual void end() = 0;
	};

	class NullPostEffect :public PostEffect
	{
	public:
		void setSlice(Rect s) override {}
		void setAlphaMod(float a) override {}
		void setColorMod(Color c) override {}
		void restore() override {}
		void begin() override {}
		void end() override {}
	};

	class SdlPostEffect :public PostEffect
	{
	public:
		SdlPostEffect() = default;
		~SdlPostEffect();
		void setSlice(Rect s) override;
		void setAlphaMod(float a) override;
		void setColorMod(Color c) override;
		void restore() override;
		void begin() override;
		void end() override;

	private:
		struct Impl;
		std::unique_ptr<Impl> m_Pimpl;
	};
}
