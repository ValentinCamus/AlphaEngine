//
// Created by Valentin on 2019-05-08.
//

#ifndef ALPHAENGINE_LOG_HPP
#define ALPHAENGINE_LOG_HPP

#include <spdlog/spdlog.h>

namespace Alpha
{
    #define LOG_TRACE       SPDLOG_TRACE
    #define LOG_DEBUG       SPDLOG_DEBUG
    #define LOG_INFO        SPDLOG_INFO
    #define LOG_WARN        SPDLOG_WARN
    #define LOG_ERROR       SPDLOG_ERROR
    #define LOG_CRITICAL    SPDLOG_CRITICAL

    enum ELogLevel
    {
        Trace = SPDLOG_LEVEL_TRACE,
        Debug = SPDLOG_LEVEL_DEBUG,
        Info = SPDLOG_LEVEL_INFO,
        Warn = SPDLOG_LEVEL_WARN,
        Error = SPDLOG_LEVEL_ERROR,
        Critical = SPDLOG_LEVEL_CRITICAL,
        Silent = SPDLOG_LEVEL_OFF,
    };

    inline void SetLogLevel(ELogLevel lvl)
    {
        auto format = static_cast<spdlog::level::level_enum>(lvl);
        spdlog::set_level(format);
    }
}




#endif //ALPHAENGINE_LOG_HPP
