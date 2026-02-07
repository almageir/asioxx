#include <detail/standalone_log_backend.h>
#include <dataio/sink/sink.h>
#include <asio/any_io_executor.hpp>

namespace asynclog {

    StandaloneLogBackend::StandaloneLogBackend()
        : idle_work_{ioc_.get_executor()}
    {
        io_thread_ = std::thread([&](){
            ioc_.run();
        });
    }

    StandaloneLogBackend::~StandaloneLogBackend()
    {
        sinks_.clear();
        idle_work_.reset();
        io_thread_.join();
    }

    void StandaloneLogBackend::addSink(SinkPtr sink)
    {
        sinks_.emplace_back(std::move(sink));
    }

    void StandaloneLogBackend::push(const std::string &message) const
    {
        if (!message.empty())
            for (const auto& sink : sinks_)
                sink->write(message);
    }

    asio::any_io_executor StandaloneLogBackend::get_executor()
    {
        return ioc_.get_executor();
    }

}