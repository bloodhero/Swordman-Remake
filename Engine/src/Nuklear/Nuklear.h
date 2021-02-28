#pragma once

namespace meow {

	class Nuklear
	{
	public:
		virtual ~Nuklear() = default;
		virtual void begin() = 0;
		virtual void end() = 0;
		virtual void eventBegin() = 0;
		virtual void eventEnd() = 0;
		virtual void render() = 0;
		virtual void* getContext() = 0;
		virtual void onEvent(void* e) = 0;
	};

	class NullNuklear :public Nuklear
	{
	public:
		~NullNuklear() = default;
		void begin() {}
		void end() {}
		void eventBegin() {}
		void eventEnd() {}
		void render() {}
		void* getContext() { return nullptr; }
		void onEvent(void* e) {}
	};

	class SdlGl3Nuklear :public Nuklear
	{
	public:
		SdlGl3Nuklear();
		~SdlGl3Nuklear() = default;
		void begin() override;
		void end() override;
		void eventBegin() override;
		void eventEnd() override;
		void render() override;
		void* getContext() override;
		void onEvent(void* e) override;
	private:
		struct Impl;
		std::unique_ptr<Impl> m_Pimpl;
	};
}
