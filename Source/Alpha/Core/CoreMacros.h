#pragma once

#ifdef _WIN64
#define PLATFORM_WINDOWS
#elif _WIN32
#define PLATFORM_WINDOWS
#elif __APPLE__
#define PLATFORM_APPLE
#elif __linux
#define PLATFORM_LINUX
#else
#define PLATFORM_UNKNOW
#endif

#ifdef ALPHA_WARN_UNUSED
#define ALPHA_UNUSED(var) Alpha::Logger::Warn("Unused variable {0}", #var);
#else
#define ALPHA_UNUSED(var) (void) var;
#endif

#ifdef ALPHA_ENABLE_ASSERT
#define ALPHA_ASSERT(expr, ...) \
	if (!(expr)) \
	{ \
		Alpha::Logger::Error(__VA_ARGS__); \
		Alpha::Logger::Info("Shutdown: AlphaEngine"); \
		Alpha::ForceQuit(); \
	} 
#else
#define ALPHA_ASSERT(expr, ...)
#endif

#ifdef ALPHA_ENABLE_CHECK
#define ALPHA_CHECK(expr, ...) \
	if (!(expr)) \
	{ \
		Alpha::Logger::Error(__VA_ARGS__); \
		Alpha::Logger::Info("Shutdown: AlphaEngine"); \
		Alpha::ForceQuit(); \
	} 
#else
#define ALPHA_CHECK(expr, ...) expr
#endif

#define ALPHA_BIND_EVENT(fn, ref) std::bind(&fn, ref, std::placeholders::_1)

#define PROJECT_SOURCE_DIR std::string("D:/AlphaEngine/")