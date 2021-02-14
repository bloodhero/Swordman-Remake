#include "Core/Log.h"

#include <cstdarg>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace meow {

	struct spdlogLog::Impl {
		std::shared_ptr<spdlog::logger> logger;
	};

	spdlogLog::spdlogLog(std::string_view filename) :
		m_Pimpl(std::make_unique<Impl>())
	{
		std::vector<spdlog::sink_ptr> logSinks;
		logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(filename.data(), true));

		logSinks[0]->set_pattern("%^[%T] %n: %v%$");
		logSinks[1]->set_pattern("[%T] [%l] %n: %v");

		m_Pimpl->logger = std::make_shared<spdlog::logger>("SWORDMAN", begin(logSinks), end(logSinks));
		spdlog::register_logger(m_Pimpl->logger);
		m_Pimpl->logger->set_level(spdlog::level::trace);
		m_Pimpl->logger->flush_on(spdlog::level::trace);
	}

	void spdlogLog::trace(const char* fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		m_Pimpl->logger->info(fmt, args);
		va_end(args);
	}


	void spdlogLog::info(const char* fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		m_Pimpl->logger->info(fmt, args);
		va_end(args);
	}


	void spdlogLog::warn(const char* fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		m_Pimpl->logger->warn(fmt, args);
		va_end(args);
	}


	void spdlogLog::error(const char* fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		m_Pimpl->logger->error(fmt, args);
		va_end(args);
	}


	void spdlogLog::critical(const char* fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		m_Pimpl->logger->critical(fmt, args);
		va_end(args);
	}

}
