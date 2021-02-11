#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "Core/Utils.h"

namespace meow {
	class MixChunk
	{
	public:
		MixChunk(std::string_view filename);
		MixChunk(void* raw_data, int size);
		std::string getName();
		~MixChunk();
	};
	class Audio
	{
	public:
		static Ref<Audio> getAudio() { return  s_AudioService; }
		static void setAudio(Ref<Audio> a);
		virtual ~Audio() = default;
		virtual void playSFX(MixChunk* chunk);
		virtual void playMusic(MixChunk* chunk, bool loop);
		virtual void playMusics(std::vector<MixChunk*> chunks, bool loop);
		virtual void playStream(std::shared_ptr<MixChunk> chunk);
		virtual void onUpdate(float time);
	private:
		static Ref<Audio> s_AudioService;
	};

	class NullAudio :public Audio
	{
	public:
		void playSFX(MixChunk* chunk) override { std::cout << "playing sfx : " << chunk->getName() << std::endl; }
		void playMusic(MixChunk* chunk, bool loop) override { std::cout << "playing music: " << chunk->getName() << std::endl; }
		void playMusics(std::vector<MixChunk*> chunks, bool loop) override { std::cout << "playing number musics: " << chunks.size() << std::endl; }
		void onUpdate(float time) override {}
		void playStream(std::shared_ptr<MixChunk> chunk) override { std::cout << "playing stream audio" << std::endl; }

	};

	class OpenALAudio :public Audio
	{
	public:
		void playSFX(MixChunk* chunk) override;
		void playMusic(MixChunk* chunk, bool loop) override;
		void playMusics(std::vector<MixChunk*> chunks, bool loop) override;
		void playStream(std::shared_ptr<MixChunk> chunk) override;
		void onUpdate(float time) override;

	private:
		struct Impl;
		std::unique_ptr<Impl> m_Pimpl;
	};
}
