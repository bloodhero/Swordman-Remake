#include "Core/Log.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/sinks/file_sinks.h>

namespace meow {


	spdlogLog::spdlogLog(std::string_view filename) :
		Log(filename)
	{

	}


	void spdlogLog::trace(...)
	{

	}


	void spdlogLog::info(...)
	{

	}


	void spdlogLog::warn(...)
	{

	}


	void spdlogLog::error(...)
	{

	}


	void spdlogLog::critical(...)
	{

	}

}
