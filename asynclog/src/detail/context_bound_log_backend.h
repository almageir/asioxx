#ifndef ASYNCLOG_LOG_MANAGER_H
#define ASYNCLOG_LOG_MANAGER_H

#include "log_backend_interface.h"

#include <asio/any_io_executor.hpp>

#include <memory>
#include <string>
#include <vector>

namespace asynclog
{

    class ContextBoundLogBackend : public LogBackendInterface
    {
    public:
        explicit ContextBoundLogBackend(asio::any_io_executor executor);
        ~ContextBoundLogBackend() override;

        void addSink(SinkPtr sink) override;
        void push(const std::string& message) const override;

        asio::any_io_executor get_executor() override;

    private:
        std::vector<SinkPtr> sinks_;
        asio::any_io_executor executor_;
    };

}

#endif // ASYNCLOG_LOG_MANAGER_H
