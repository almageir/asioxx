#include <iostream>
#include <asynclog/logger_factory.h>

namespace asynclog {

    LoggerFactory::LoggerFactory(std::shared_ptr<LogManagerInterface> log_backend)
        : log_backend_{std::move(log_backend)}
    {
    }

    LoggerFactory::~LoggerFactory() = default;

    ScopedLogger LoggerFactory::create(std::string_view name) const
    {
        return {name, log_backend_};
    }
}
