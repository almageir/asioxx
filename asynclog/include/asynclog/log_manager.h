#ifndef ASYNCLOG_LOG_MANAGER_H
#define ASYNCLOG_LOG_MANAGER_H

#include <memory>
#include <string>

#include <asio/io_context.hpp>
#include <asio/any_io_executor.hpp>
#include <thread>

namespace dataio {
    class Sink;
}

namespace asynclog
{
    using SinkPtr = std::shared_ptr<dataio::Sink>;

    class LogManager
    {
    public:
        LogManager();
        ~LogManager();

        void addSink(SinkPtr sink);
        void push(const std::string& message) const;

        LogManager(const LogManager&) = delete;
        LogManager(LogManager&&) = delete;
        LogManager& operator=(const LogManager&) = delete;
        LogManager& operator=(LogManager&&) = delete;

        asio::any_io_executor get_executor();

    private:
        asio::io_context ioc_;
        asio::executor_work_guard<asio::io_context::executor_type> idle_work_;
        std::thread io_thread_;
        std::vector<SinkPtr> sinks_;
    };
}


#endif // ASYNCLOG_LOG_MANAGER_H
