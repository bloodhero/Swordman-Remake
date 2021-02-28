#pragma once

#include <string>
#include <memory>

namespace meow {
	class Video
	{
	public:
		virtual void skip() = 0;
		virtual bool isEnd() = 0;
		virtual std::string getName() = 0;
	};

}
