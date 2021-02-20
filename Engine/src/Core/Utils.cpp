#include "pch.h"
#include "Core/Utils.h"

namespace meow {

	std::string dirName(std::string source)
	{
		if (source.size() <= 1) //Make sure it's possible to check the last character.
		{
			return source;
		}
		if (*(source.rbegin() + 1) == '/') //Remove trailing slash if it exists.
		{
			source.pop_back();
		}
		source.erase(std::find(source.rbegin(), source.rend(), '/').base(), source.end());
		return source;
	}
}
