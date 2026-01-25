#include <asynclog/log_manager.h>

#include <dataio/sink/console_sink.h>
#include <dataio/sink/file_sink.h>

namespace asynclog
{
    LogManager::LogManager()
        : idle_work_{ioc_.get_executor()}
    {
        io_thread_ = std::thread([&](){
            ioc_.run();
        });
    }

    LogManager::~LogManager()
    {
        sinks_.clear();
        idle_work_.reset();
        io_thread_.join();
    }

    void LogManager::addSink(SinkPtr sink)
    {
        sinks_.emplace_back(std::move(sink));
    }

    void LogManager::push(const std::string &message) const
    {
        if (!message.empty())
            for (auto &sink : sinks_)
                sink->write(message);
    }

    asio::any_io_executor LogManager::get_executor()
    {
        return ioc_.get_executor();
    }
}
