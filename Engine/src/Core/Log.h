#pragma once

#include <string>
#include <memory>


namespace meow {

	class Log
	{
	public:
		virtual ~Log() = default;
		virtual void trace(const char* fmt, ...) = 0;
		virtual void info(const char* fmt, ...) = 0;
		virtual void warn(const char* fmt, ...) = 0;
		virtual void error(const char* fmt, ...) = 0;
		virtual void critical(const char* fmt, ...) = 0;
	};

	class NullLog :public Log
	{
	public:
		void trace(const char* fmt, ...) override {}
		void info(const char* fmt, ...) override {}
		void warn(const char* fmt, ...) override {}
		void error(const char* fmt, ...) override {}
		void critical(const char* fmt, ...) override {}

	};

	class spdlogLog :public Log
	{
	public:
		spdlogLog(std::string_view filename);
		void trace(const char* fmt, ...) override;
		void info(const char* fmt, ...) override;
		void warn(const char* fmt, ...) override;
		void error(const char* fmt, ...) override;
		void critical(const char* fmt, ...) override;

	private:
		struct Impl;
		std::unique_ptr<Impl> m_Pimpl;
	};
}
