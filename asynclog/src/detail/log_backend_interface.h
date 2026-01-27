#ifndef MTLS_TUN_LOG_MANAGER_INTERFACE_H
#define MTLS_TUN_LOG_MANAGER_INTERFACE_H

#include <memory>
#include <string>

namespace dataio {
    class Sink;
}

namespace asio {
    class any_io_executor;
}

namespace asynclog {

    using SinkPtr = std::shared_ptr<dataio::Sink>;

    class LogBackendInterface
    {
    public:
        LogBackendInterface() = default;
        virtual ~LogBackendInterface() = default;

        virtual void addSink(SinkPtr sink) = 0;
        virtual void push(const std::string& message) const = 0;
        virtual asio::any_io_executor get_executor() = 0;

        LogBackendInterface(const LogBackendInterface&) = delete;
        LogBackendInterface(LogBackendInterface&&) = delete;
        LogBackendInterface& operator=(const LogBackendInterface&) = delete;
        LogBackendInterface& operator=(LogBackendInterface&&) = delete;
    };

    using LogBackendPtr = std::unique_ptr<LogBackendInterface>;

}

#endif //MTLS_TUN_LOG_MANAGER_INTERFACE_H