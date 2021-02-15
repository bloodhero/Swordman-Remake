#include "pch.h"
#include <cstdarg>
#include "Core/Log.h"
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

		logSinks[0]->set_pattern("[%T] [%l] %n: %v");
		logSinks[1]->set_pattern("[%T] [%l]: %v");

		m_Pimpl->logger = std::make_shared<spdlog::logger>(filename.data(), begin(logSinks), end(logSinks));
		spdlog::register_logger(m_Pimpl->logger);
		m_Pimpl->logger->set_level(spdlog::level::trace);
		m_Pimpl->logger->flush_on(spdlog::level::trace);
	}


	void spdlogLog::log_message(LogLevel level, std::string_view message)
	{
		switch (level)
		{
		case Log::LogLevel::trace:
			m_Pimpl->logger->trace(message.data());
			break;
		case Log::LogLevel::info:
			m_Pimpl->logger->info(message.data());
			break;
		case Log::LogLevel::warn:
			m_Pimpl->logger->warn(message.data());
			break;
		case Log::LogLevel::error:
			m_Pimpl->logger->error(message.data());
			break;
		case Log::LogLevel::critical:
			m_Pimpl->logger->critical(message.data());
			break;
		default:
			break;
		}
	}

}
