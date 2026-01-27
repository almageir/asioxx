#ifndef MTLS_TUN_LOG_MANAGER_STANDALONE_H
#define MTLS_TUN_LOG_MANAGER_STANDALONE_H

#include "log_backend_interface.h"

#include <asio/io_context.hpp>
#include <asio/any_io_executor.hpp>
#include <thread>

#include <memory>
#include <string>
#include <vector>

namespace asynclog {

    class StandaloneLogBackend : public LogBackendInterface
    {
    public:
        StandaloneLogBackend();
        ~StandaloneLogBackend() override;

        void addSink(SinkPtr sink) override;
        void push(const std::string& message) const override;

        asio::any_io_executor get_executor() override;

    private:
        asio::io_context ioc_;
        asio::executor_work_guard<asio::io_context::executor_type> idle_work_;
        std::thread io_thread_;
        std::vector<SinkPtr> sinks_;
    };

}

#endif //MTLS_TUN_LOG_MANAGER_STANDALONE_H