#include <asynclog/logger.h>
#include <asynclog/log_manager.h>
#include <asynclog/log_provider.h>

namespace asynclog
{
    LogProvider& Logger::get_instance()
    {
        static LogProvider impl_;
        return impl_;
    }

    void Logger::initialize(std::string_view path)
    {
        return get_instance().open(path);
    }

    void Logger::write(std::string_view msg, MsgType type, const std::string& from)
    {
        return get_instance().write(msg, type, from);
    }
}

