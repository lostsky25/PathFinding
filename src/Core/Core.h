#pragma once

#include "Core/Log.h"

#ifdef PF_DEBUG
	#define PF_ENABLE_ASSERTS
#endif

#ifdef PF_ENABLE_ASSERTS
	#define PF_ASSERT(x, ...) { if(!(x)) { PF_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define PF_CORE_ASSERT(x, ...) { if(!(x)) { PF_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define PF_ASSERT(x, ...)
	#define PF_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)