#pragma once

#include "Core/Log.h"
#include "Manager.h"

#define BIT(x) (1 << x)

#ifdef ENGINE
#define LOGGER Manager::getManager()->getCoreLog()
#else
#define LOGGER Manager::getManager()->getClientLog()
#endif

#ifdef DEBUG
#define DEBUGBREAK()         __debugbreak()
#define ASSERT(x, ...) { if(!(x)) { LOGGER->error(__VA_ARGS__); DEBUGBREAK(); } }
#else
#define DEBUGBREAK()
#define ASSERT(x, ...)
#endif
