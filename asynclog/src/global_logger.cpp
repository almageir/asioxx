#include <asynclog/global_logger.h>
#include <asynclog/log_manager.h>
#include "detail/standalone_log_backend.h"

namespace asynclog
{
    LogManager& GlobalLogger::get_instance()
    {
        static LogManager impl_;
        return impl_;
    }

    void GlobalLogger::initialize(LogMode mode)
    {
        return get_instance().open(mode, {});
    }

    void GlobalLogger::initialize(LogMode mode, std::string_view path)
    {
        return get_instance().open(mode, path);
    }

    void GlobalLogger::write(std::string_view msg, MsgType type, const std::string& from)
    {
        return get_instance().write(msg, type, from);
    }
}

