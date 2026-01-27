#ifndef ASYNCLOG_GLOBAL_LOGGER_H
#define ASYNCLOG_GLOBAL_LOGGER_H

#include "log_manager_interface.h"

#include <string>
#include <string_view>

namespace asynclog
{
    class LogManager;

    class GlobalLogger
    {
    public:
        static void initialize(LogMode mode);
        static void initialize(LogMode mode, std::string_view path);
        static void write(std::string_view msg, MsgType type, const std::string& from);

    private:
        static LogManager& get_instance();
    };

    inline void info(std::string_view msg, const std::string& from = {})
    {
        GlobalLogger::write(msg, MsgType::EInfo, from);
    }

    inline void debug(std::string_view msg, const std::string& from = {})
    {
        GlobalLogger::write(msg, MsgType::EDebug, from);
    }

    inline void warn(std::string_view msg, const std::string& from = {})
    {
        GlobalLogger::write(msg, MsgType::EWarn, from);
    }

    inline void err(std::string_view msg, const std::string& from = {})
    {
        GlobalLogger::write(msg, MsgType::EError, from);
    }
}

#endif // ASYNCLOG_GLOBAL_LOGGER_H
