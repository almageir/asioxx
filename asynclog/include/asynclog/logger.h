#ifndef ASYNCLOG_LOGGER_H
#define ASYNCLOG_LOGGER_H

#include "common.h"

#include <string>
#include <string_view>

namespace asynclog
{
    class LogManager;
    class LogProvider;

    class Logger
    {
    public:
        static void initialize(std::string_view path = "trace.log");
        static void write(std::string_view msg, MsgType type, const std::string& from);
        static bool is_enabled();
        static void enable();
        static void disable();

    private:
        static LogProvider& get_instance();
    };

    inline void info(std::string_view msg, const std::string& from = {})
    {
        Logger::write(msg, MsgType::EInfo, from);
    }

    inline void debug(std::string_view msg, const std::string& from = {})
    {
        Logger::write(msg, MsgType::EDebug, from);
    }

    inline void warn(std::string_view msg, const std::string& from = {})
    {
        Logger::write(msg, MsgType::EWarn, from);
    }

    inline void err(std::string_view msg, const std::string& from = {})
    {
        Logger::write(msg, MsgType::EError, from);
    }
}

#endif // ASYNCLOG_LOGGER_H
