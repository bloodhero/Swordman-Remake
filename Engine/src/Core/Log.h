#pragma once

#include <string>
#include <memory>
#include <spdlog/fmt/fmt.h>


namespace meow {

	class Log
	{
	public:
		enum class LogLevel
		{
			trace,
			info,
			warn,
			error,
			critical
		};

		template<typename... Args>
		void trace(Args... args)
		{
			std::string message = fmt::format(std::forward<Args>(args)...);
			log_message(LogLevel::trace, message);
		}

		template<typename... Args>
		void info(Args... args)
		{
			std::string message = fmt::format(std::forward<Args>(args)...);
			log_message(LogLevel::info, message);
		}

		template<typename... Args>
		void warn(Args... args)
		{
			sstd::string message = fmt::format(std::forward<Args>(args)...);
			log_message(LogLevel::warn, message);
		}

		template<typename... Args>
		void error(Args... args)
		{
			std::string message = fmt::format(std::forward<Args>(args)...);
			log_message(LogLevel::error, message);
		}

		template<typename... Args>
		void critical(Args... args)
		{
			std::string message = fmt::format(std::forward<Args>(args)...);
			log_message(LogLevel::critical, message);
		}

	protected:
		virtual void log_message(LogLevel level, std::string_view message) = 0;
	};


	class NullLog :public Log
	{
	protected:
		void log_message(LogLevel level, std::string_view message) override {}

	};

	class spdlogLog :public Log
	{
	public:
		spdlogLog(std::string_view filename);
		~spdlogLog() = default;

	private:
		struct Impl;
		std::unique_ptr<Impl> m_Pimpl;

	protected:
		void log_message(LogLevel level, std::string_view message) override;

	};
}
