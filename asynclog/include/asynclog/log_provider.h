#ifndef ASYNCLOG_LOG_PROVIDER_H
#define ASYNCLOG_LOG_PROVIDER_H

#include "common.h"

#include <atomic>
#include <memory>
#include <string_view>

namespace asynclog
{
    class LogManager;

    class LogProvider
    {
    public:
        LogProvider();
        void open(std::string_view path) const;
        void write(std::string_view msg, MsgType type, std::string_view name) const;

    private:
        std::unique_ptr<LogManager> log_backend_;
    };
}

#endif // ASYNCLOG_LOG_PROVIDER_H