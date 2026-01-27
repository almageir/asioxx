#include <detail/context_bound_log_backend.h>

#include <dataio/sink/console_sink.h>
#include <dataio/sink/file_sink.h>

namespace asynclog
{
    ContextBoundLogBackend::ContextBoundLogBackend(asio::any_io_executor executor)
        : executor_(std::move(executor))
    {}

    ContextBoundLogBackend::~ContextBoundLogBackend()
    {
        sinks_.clear();
    }

    void ContextBoundLogBackend::addSink(SinkPtr sink)
    {
        sinks_.emplace_back(std::move(sink));
    }

    void ContextBoundLogBackend::push(const std::string &message) const
    {
        if (!message.empty())
            for (auto &sink : sinks_)
                sink->write(message);
    }

    asio::any_io_executor ContextBoundLogBackend::get_executor()
    {
        return executor_;
    }
}
