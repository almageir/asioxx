#ifndef ASYNCLOG_LOG_PROVIDER_H
#define ASYNCLOG_LOG_PROVIDER_H

#include "log_manager_interface.h"

#include <memory>
#include <string_view>

namespace asio {
    class any_io_executor;
}

namespace asynclog
{
    class LogBackendInterface;

    class LogManager : public LogManagerInterface
    {
    public:
        LogManager();
        explicit LogManager(asio::any_io_executor executor);
        ~LogManager() override;
        void open(LogMode mode) override;
        void open(LogMode mode, std::string_view path) override;
        void write(std::string_view msg, MsgType type, std::string_view name) const override;

    private:
        std::unique_ptr<LogBackendInterface> log_backend_;
    };
}

#endif // ASYNCLOG_LOG_PROVIDER_H