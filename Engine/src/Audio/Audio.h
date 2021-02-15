#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "Core/Utils.h"

namespace meow {
	class MixChunk
	{
	public:
		virtual ~MixChunk() = default;
		virtual std::string getName() = 0;

	};

	class OpenALMixChunk :public MixChunk
	{
	public:

	};

	class Audio
	{
	public:
		virtual ~Audio() = default;
		virtual void playSFX(MixChunk* chunk) = 0;
		virtual void playMusic(MixChunk* chunk, bool loop) = 0;
		virtual void playMusics(std::vector<MixChunk*> chunks, bool loop) = 0;
		virtual void playStream(std::unique_ptr<MixChunk> chunk) = 0;
		virtual void onUpdate(float time) = 0;
	};

	class NullAudio :public Audio
	{
	public:
		~NullAudio() = default;
		void playSFX(MixChunk* chunk) override { std::cout << "playing sfx : " << chunk->getName() << std::endl; }
		void playMusic(MixChunk* chunk, bool loop) override { std::cout << "playing music: " << chunk->getName() << std::endl; }
		void playMusics(std::vector<MixChunk*> chunks, bool loop) override { std::cout << "playing number musics: " << chunks.size() << std::endl; }
		void onUpdate(float time) override {}
		void playStream(std::unique_ptr<MixChunk> chunk) override { std::cout << "playing stream audio" << std::endl; }

	};

	class OpenALAudio :public Audio
	{
	public:
		void playSFX(MixChunk* chunk) override;
		void playMusic(MixChunk* chunk, bool loop) override;
		void playMusics(std::vector<MixChunk*> chunks, bool loop) override;
		void playStream(std::unique_ptr<MixChunk> chunk) override;
		void onUpdate(float time) override;

	private:
		struct Impl;
		std::unique_ptr<Impl> m_Pimpl;
	};
}
