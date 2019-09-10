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
#define ALPHA_UNUSED(_var_) Alpha::Logger::Warn("Unused variable {0}", #_var_);
#else
#define ALPHA_UNUSED(_var_) (void) _var_;
#endif

#ifdef ALPHA_ENABLE_ASSERT
#define ALPHA_ASSERT(_expr_, _msg_) \
	if (!(_expr_)) \
	{ \
		Alpha::Logger::Error(std::string("[Assert] ") + std::string(_msg_)); \
		Alpha::Logger::Info("Shutdown: AlphaEngine"); \
		Alpha::ForceQuit(); \
	} 
#else
#define ALPHA_ASSERT(_expr_, _msg_)
#endif

#ifdef ALPHA_ENABLE_CHECK
#define ALPHA_CHECK(_expr_, _msg_) \
	if (!(_expr_)) \
	{ \
		Alpha::Logger::Error(std::string("[Check] ") + std::string(_msg_)); \
		Alpha::Logger::Info("Shutdown: AlphaEngine"); \
		Alpha::ForceQuit(); \
	} 
#else
#define ALPHA_CHECK(_expr_, _msg_) _expr_
#endif

#define ALPHA_BIND_EVENT(_fn_, _ref_) std::bind(&_fn_, _ref_, std::placeholders::_1)