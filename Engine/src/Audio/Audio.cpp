#include "pch.h"
#include "Audio/Audio.h"
#include <SFML/System/Vector2.hpp>
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>
extern "C" {
#include <libavutil/avutil.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>
}
using namespace sf;

namespace meow {

	class AudioDecoder
	{
	public:

		AudioDecoder(std::string_view filename);
		~AudioDecoder() = default;
		void* getData() { return m_Data.data(); }
		int getSize() { return m_Data.size(); }
		int getSampleRate() { return m_SampleRate; }
		int getChannels() { return m_Channel; }
		AVSampleFormat getFormat() { return m_Format; }

	private:
		std::vector<uint8_t> m_Data;
		int m_Channel = 0;
		int m_SampleRate = 0;
		AVSampleFormat m_Format;
		AVSampleFormat convertFormat(AVSampleFormat format);
	};

	AudioDecoder::AudioDecoder(std::string_view filename)
	{
		AVFormatContext* format_context;
		format_context = avformat_alloc_context();

		avformat_open_input(&format_context, filename.data(), NULL, NULL);
		avformat_find_stream_info(format_context, nullptr);

		int audio_stream_index = -1;
		AVStream* av_stream;

		for (unsigned i = 0; i < format_context->nb_streams; ++i)
		{
			av_stream = format_context->streams[i];
			if (av_stream->codecpar->codec_type == AVMEDIA_TYPE_AUDIO)
			{
				audio_stream_index = i;
				break;
			}
		}

		if (audio_stream_index == -1)
		{
			LOGGER->error("Failed to find codec, {} may be not a audio file!", filename);
			return;
		}

		AVCodecParameters* codec_params;
		codec_params = av_stream->codecpar;

		AVCodec* codec;
		codec = avcodec_find_decoder(codec_params->codec_id);

		AVCodecContext* codec_context;
		codec_context = avcodec_alloc_context3(codec);

		avcodec_parameters_to_context(codec_context, codec_params);
		avcodec_open2(codec_context, codec, nullptr);

		if (codec_params->channel_layout == 0)
		{
			codec_params->channel_layout = av_get_default_channel_layout(codec_params->channels);
		}

		m_Format = convertFormat(static_cast<AVSampleFormat>(codec_params->format));
		m_SampleRate = codec_params->sample_rate;
		m_Channel = codec_params->channels;

		SwrContext* swr_context = swr_alloc();
		swr_alloc_set_opts(
			swr_context,
			codec_params->channel_layout,
			m_Format,
			m_SampleRate,
			codec_params->channel_layout,
			static_cast<AVSampleFormat>(codec_params->format),
			codec_params->sample_rate,
			0,
			nullptr);
		swr_init(swr_context);

		AVPacket* packet = av_packet_alloc();
		AVFrame* frame = av_frame_alloc();

		while (av_read_frame(format_context, packet) >= 0)
		{
			avcodec_send_packet(codec_context, packet);
			int ret = avcodec_receive_frame(codec_context, frame);

			if (ret == AVERROR(EAGAIN)) {
				continue;
			}

			uint8_t* buffer;
			av_samples_alloc(&buffer, NULL, m_Channel, frame->nb_samples, m_Format, NULL);
			swr_convert(swr_context, &buffer, frame->nb_samples, (const uint8_t **)(frame->data), frame->nb_samples);
			size_t size = av_samples_get_buffer_size(nullptr, m_Channel, frame->nb_samples, m_Format, 1);
			m_Data.insert(m_Data.end(), buffer, buffer + size);
			av_freep(&buffer);
		}

		// flush codec
		avcodec_send_packet(codec_context, NULL);
		while (avcodec_receive_frame(codec_context, frame) != AVERROR_EOF)
			;
		avcodec_flush_buffers(codec_context);

		swr_close(swr_context);
		swr_free(&swr_context);

		av_frame_free(&frame);
		av_packet_free(&packet);

		avcodec_free_context(&codec_context);
		avcodec_close(codec_context);

		avformat_close_input(&format_context);
		avformat_free_context(format_context);

	}


	AVSampleFormat AudioDecoder::convertFormat(AVSampleFormat format)
	{
		switch (format) {
		case AV_SAMPLE_FMT_U8P:
			return AV_SAMPLE_FMT_U8;
		case AV_SAMPLE_FMT_S16P:
			return AV_SAMPLE_FMT_S16;
		case AV_SAMPLE_FMT_S32P:
			return AV_SAMPLE_FMT_FLT;
		case AV_SAMPLE_FMT_S64P:
			return AV_SAMPLE_FMT_DBL;
		case AV_SAMPLE_FMT_FLTP:
			return AV_SAMPLE_FMT_FLT;
		case AV_SAMPLE_FMT_DBLP:
			return AV_SAMPLE_FMT_DBL;
		default:
			return format;
		}
	}

	class OpenALListener
	{
	public:
		void setPosition(Vector2f m_Position);

	};

	void OpenALListener::setPosition(Vector2f m_Position)
	{
		alListener3f(AL_POSITION, m_Position.x, m_Position.y, 0.0f);
	}

	class OpenALSource
	{
	public:
		OpenALSource();
		~OpenALSource();
		void setPosition(Vector2f m_Position);
		void setVolume(float volume);
		void play(MixChunk* chunk);
		void stop();
		bool isPlaying();

	private:
		ALuint m_SourceID = 0;
		ALuint m_BufferID = 0;
		Vector2f m_Position;
	};

	OpenALSource::OpenALSource()
	{
		alGetError();
		alGenSources(1, &m_SourceID);
		const ALenum error = alGetError();
		ASSERT(error == AL_NO_ERROR, "alGenSources failed: {}", error);

		alSourcef(m_SourceID, AL_GAIN, 1.0f);
		alSourcef(m_SourceID, AL_PITCH, 1.0f);
		alSource3f(m_SourceID, AL_POSITION, 0.0f, 0.0f, 0.0f);
	}

	OpenALSource::~OpenALSource()
	{
		stop();
		alSourcei(m_SourceID, AL_BUFFER, AL_NONE);
		alDeleteSources(1, &m_SourceID);
	}

	void OpenALSource::setPosition(Vector2f m_Position)
	{
		alSource3f(m_SourceID, AL_POSITION, m_Position.x, m_Position.y, 0.f);
	}

	void OpenALSource::setVolume(float volume)
	{
		alSourcef(m_SourceID, AL_GAIN, volume);
	}

	void OpenALSource::play(MixChunk* chunk)
	{
		alGetError();
		m_BufferID = *static_cast<ALuint*>(chunk->getData());
		alSourcei(m_SourceID, AL_BUFFER, m_BufferID);
		alSourcePlay(m_SourceID);
		const ALenum error = alGetError();
		ASSERT(error == AL_NO_ERROR, "source play failed: {}", error);
	}

	void OpenALSource::stop()
	{
		alSourceStop(m_SourceID);
	}

	bool OpenALSource::isPlaying()
	{
		ALint state;
		alGetSourcei(m_SourceID, AL_SOURCE_STATE, &state);
		return state == AL_PLAYING;
	}

	class OpenALBuffer
	{
	public:
		OpenALBuffer(void* data, int size, int sample_rate, AVSampleFormat format, int channels);
		~OpenALBuffer();

		ALuint getBufferId() const { return m_BufferID; }
	private:
		ALenum formatConvert(AVSampleFormat format, int channel);
		ALuint m_BufferID;
	};

	OpenALBuffer::OpenALBuffer(void* data, int size, int sample_rate, AVSampleFormat format, int channels)
	{
		alGetError();
		alGenBuffers(1, &m_BufferID);
		auto error = alGetError();
		ASSERT(error == AL_NO_ERROR, "alGenBuffers failed: {}", error);

		alGetError();
		alBufferData(m_BufferID, formatConvert(format, channels), (const ALvoid*)data, size, sample_rate);
		error = alGetError();
		ASSERT(error == AL_NO_ERROR, "alBuffers submit data failed: {}", error);
	}

	OpenALBuffer::~OpenALBuffer()
	{
		alDeleteBuffers(1, &m_BufferID);
	}

	ALenum OpenALBuffer::formatConvert(AVSampleFormat format, int channel)
	{
		switch (format)
		{
		case AV_SAMPLE_FMT_U8:
			return (channel == 2) ? AL_FORMAT_STEREO8 : AL_FORMAT_MONO8;
			break;
		case AV_SAMPLE_FMT_S16:
			return (channel == 2) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
			break;
		case AV_SAMPLE_FMT_S32:
			return (channel == 2) ? AL_FORMAT_STEREO_FLOAT32 : AL_FORMAT_MONO_FLOAT32;
			break;
		case AV_SAMPLE_FMT_FLT:
			return (channel == 2) ? AL_FORMAT_STEREO_FLOAT32 : AL_FORMAT_MONO_FLOAT32;
			break;
		case AV_SAMPLE_FMT_DBL:
			return (channel == 2) ? AL_FORMAT_STEREO_DOUBLE_EXT : AL_FORMAT_MONO_DOUBLE_EXT;
			break;
		default:
			ASSERT(false, "{}: unknown buffer format {}", __func__, format);
			break;
		}
	}

	class MusicPlayer
	{
	public:
		void play(MixChunk* chunk, bool loop);
		void play(std::vector<MixChunk*> chunks, bool loop);
		void stop();
		void onUpdate();
		void setVolume(float volume);

	private:
		OpenALSource m_Source;
		std::vector<MixChunk*> m_Chunks;
		std::size_t m_Index = 0;
		bool m_IsLoop = false;
	};

	void MusicPlayer::play(MixChunk* chunk, bool loop)
	{
		std::vector<MixChunk*> temp;
		temp.push_back(chunk);
		play(temp, loop);
	}

	void MusicPlayer::play(std::vector<MixChunk*> chunks, bool loop)
	{
		m_Chunks = chunks;
		m_Index = 0;
		m_IsLoop = loop;
		m_Source.stop();
		m_Source.play(m_Chunks[0]);
	}

	void MusicPlayer::stop()
	{
		m_Chunks.clear();
		m_Index = 0;
		m_IsLoop = false;
		m_Source.stop();
	}

	void MusicPlayer::onUpdate()
	{
		if (!m_Source.isPlaying() && !m_Chunks.empty())
		{
			++m_Index;

			if (m_Index >= m_Chunks.size())
			{
				m_Index = 0;
				if (m_IsLoop)
				{
					m_Source.play(m_Chunks[m_Index]);
				}
			}
			else
			{
				m_Source.play(m_Chunks[m_Index]);
			}
		}
	}

	void MusicPlayer::setVolume(float volume)
	{
		m_Source.setVolume(volume);
	}

	class SFXPlayer
	{
	public:
		void play(MixChunk* chunk);
		void setVolume(float volume);

	private:
		static const int NUM_SOURCES = 32;
		std::array<OpenALSource, NUM_SOURCES> m_SfxSource;
	};

	void SFXPlayer::play(MixChunk* chunk)
	{
		for (auto& source : m_SfxSource)
		{
			if (!source.isPlaying())
			{
				source.play(chunk);
				break;
			}
		}
	}

	void SFXPlayer::setVolume(float volume)
	{
		for (auto& source : m_SfxSource)
		{
			source.setVolume(volume);
		}
	}

	struct OpenALAudio::Impl
	{
		Impl();
		~Impl();
		ALCdevice* device;
		ALCcontext* context;
		SFXPlayer* sfxPlayer;
		MusicPlayer* musicPlayer;
		void playSFX(MixChunk* chunk);
		void playMusic(MixChunk* chunk, bool loop);
		void playMusic(std::vector<MixChunk*> chunks, bool loop);
		void stopMusic();
		void setMusicVolume(float volume);
		void setSFXVolume(float volume);
		void playStream(std::unique_ptr<MixChunk> chunk);
		void onUpdate(float time);
	};

	OpenALAudio::Impl::Impl()
	{
		device = alcOpenDevice(nullptr);
		ASSERT(device, "alcOpenDevice failed: {}", alGetError());

		std::string device_name = alcGetString(device, ALC_DEVICE_SPECIFIER);

		context = alcCreateContext(device, nullptr);
		if (context == nullptr)
		{
			alcCloseDevice(device);
			ASSERT(context, "alcCreateContext failed: {}", alGetError());
		}

		if (!alcMakeContextCurrent(context))
		{
			alcDestroyContext(context);
			alcCloseDevice(device);
			ASSERT(false, "alcMakeContextCurrent failed: {}", alGetError());
		}

		alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);

		LOGGER->trace("AudioDevice opened!");

		sfxPlayer = new SFXPlayer();
		musicPlayer = new MusicPlayer();
	}

	OpenALAudio::Impl::~Impl()
	{
		delete sfxPlayer;
		delete musicPlayer;

		alcDestroyContext(context);

		auto result = alcCloseDevice(device);
		ASSERT(result, "alcCloseDevice failed: {}", alGetError());

		LOGGER->trace("AudioDevice closed!");
	}

	void OpenALAudio::Impl::playSFX(MixChunk* chunk)
	{
		sfxPlayer->play(chunk);
	}

	void OpenALAudio::Impl::playMusic(MixChunk* chunk, bool loop)
	{
		musicPlayer->play(chunk, loop);
	}

	void OpenALAudio::Impl::playStream(std::unique_ptr<MixChunk> chunk)
	{

	}

	void OpenALAudio::Impl::playMusic(std::vector<MixChunk*> chunks, bool loop)
	{
		musicPlayer->play(chunks, loop);
	}

	void OpenALAudio::Impl::onUpdate(float time)
	{
		musicPlayer->onUpdate();
	}

	void OpenALAudio::Impl::stopMusic()
	{
		musicPlayer->stop();
	}

	void OpenALAudio::Impl::setMusicVolume(float volume)
	{
		musicPlayer->setVolume(volume);
	}

	void OpenALAudio::Impl::setSFXVolume(float volume)
	{
		sfxPlayer->setVolume(volume);
	}

	OpenALAudio::OpenALAudio() :
		m_Pimpl(std::make_unique<Impl>())
	{

	}

	void OpenALAudio::playSFX(MixChunk* chunk)
	{
		m_Pimpl->playSFX(chunk);
	}

	void OpenALAudio::playMusic(MixChunk* chunk, bool loop)
	{
		m_Pimpl->playMusic(chunk, loop);
	}

	void OpenALAudio::playMusic(std::vector<MixChunk*> chunks, bool loop)
	{
		m_Pimpl->playMusic(chunks, loop);
	}

	void OpenALAudio::playStream(std::unique_ptr<MixChunk> chunk)
	{

	}

	void OpenALAudio::onUpdate(float time)
	{
		m_Pimpl->onUpdate(time);
	}


	void OpenALAudio::setSFXVolume(float volume)
	{
		m_Pimpl->setSFXVolume(volume);
	}


	void OpenALAudio::setMusicVolume(float volume)
	{
		m_Pimpl->setMusicVolume(volume);
	}

	struct OpenALMixChunk::Impl
	{
		Impl(std::string_view filename);
		~Impl();
		OpenALBuffer* buffer;
		ALint bufferID;
		std::string filename;
	};

	OpenALMixChunk::Impl::Impl(std::string_view f)
	{
		filename = f;

		AudioDecoder decoder(filename);
		buffer = new OpenALBuffer(decoder.getData(), decoder.getSize(), decoder.getSampleRate(), decoder.getFormat(), decoder.getChannels());
		bufferID = buffer->getBufferId();
	}

	OpenALMixChunk::Impl::~Impl()
	{
		delete buffer;
	}

	OpenALMixChunk::OpenALMixChunk(std::string_view filename) :
		m_Pimpl(std::make_unique<Impl>(filename))
	{

	}

	OpenALMixChunk::~OpenALMixChunk()
	{

	}

	std::string OpenALMixChunk::getName()
	{
		return m_Pimpl->filename;
	}

	void* OpenALMixChunk::getData()
	{
		return &(m_Pimpl->bufferID);
	}

}

