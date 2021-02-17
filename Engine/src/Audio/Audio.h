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
		virtual void* getData() = 0;

	};

	class OpenALMixChunk :public MixChunk
	{
	public:
		OpenALMixChunk(std::string_view filename);
		~OpenALMixChunk();
		std::string getName() override;
		void* getData() override;

	private:
		struct Impl;
		std::unique_ptr<Impl> m_Pimpl;
	};

	class Audio
	{
	public:
		virtual ~Audio() = default;
		virtual void playSFX(MixChunk* chunk) = 0;
		virtual void playMusic(MixChunk* chunk, bool loop) = 0;
		virtual void playMusic(std::vector<MixChunk*> chunks, bool loop) = 0;
		virtual void playStream(std::unique_ptr<MixChunk> chunk) = 0;
		virtual void setSFXVolume(float volume) = 0;
		virtual void setMusicVolume(float volume) = 0;
		virtual void onUpdate(float time) = 0;
	};

	class NullAudio :public Audio
	{
	public:
		~NullAudio() = default;
		void playSFX(MixChunk* chunk) override {}
		void playMusic(MixChunk* chunk, bool loop) override {}
		void playMusic(std::vector<MixChunk*> chunks, bool loop) override {}
		void onUpdate(float time) override {}
		void playStream(std::unique_ptr<MixChunk> chunk) override {}
		void setSFXVolume(float volume) override {}
		void setMusicVolume(float volume) override {}

	};

	class OpenALAudio :public Audio
	{
	public:
		OpenALAudio();
		~OpenALAudio() = default;
		void playSFX(MixChunk* chunk) override;
		void playMusic(MixChunk* chunk, bool loop) override;
		void playMusic(std::vector<MixChunk*> chunks, bool loop) override;
		void playStream(std::unique_ptr<MixChunk> chunk) override;
		void onUpdate(float time) override;
		void setSFXVolume(float volume) override;
		void setMusicVolume(float volume) override;

	private:
		struct Impl;
		std::unique_ptr<Impl> m_Pimpl;
	};



}
