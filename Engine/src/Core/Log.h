#pragma once

#include <string>


namespace meow {

	class Log
	{
	public:
		Log(std::string_view filename) { m_Filename = filename; }
		virtual ~Log() = default;
		virtual void trace(...) = 0;
		virtual void info(...) = 0;
		virtual void warn(...) = 0;
		virtual void error(...) = 0;
		virtual void critical(...) = 0;
	protected:
		std::string m_Filename;
	};

	class NullLog :public Log
	{
	public:
		NullLog(std::string_view filename) :Log(filename) {}
		void trace(...) override {}
		void info(...) override {}
		void warn(...) override {}
		void error(...) override {}
		void critical(...) override {}

	};

	class spdlogLog :public Log
	{
	public:
		spdlogLog(std::string_view filename);
		void trace(...) override;
		void info(...) override;
		void warn(...) override;
		void error(...) override;
		void critical(...) override;

	};
}
