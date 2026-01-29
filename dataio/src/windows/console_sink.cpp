#include <dataio/sink/console_sink.h>

#include <asio/post.hpp>

#include <iostream>

namespace dataio {

    namespace net = asio;

    ConsoleSink::ConsoleSink(net::any_io_executor executor)
        : handle_{std::cout}
        , executor_{std::move(executor)}
    {}

    ConsoleSink::~ConsoleSink() {
        std::unique_lock lock{mutex_};
        cv_.wait(lock, [this]{ return write_queue_.empty(); });
        is_stopped_ = true;
    }

    bool ConsoleSink::write(std::string buffer)
    {
        if (buffer.empty())
            return false;

        net::post(
            executor_,
            [this, self{shared_from_this()}, buf = std::move(buffer)]() mutable {
                do_write(std::move(buf));
            });

        return true;
    }

    void ConsoleSink::handle_write_queue()
    {
        if (is_stopped_)
            return;

        if (write_queue_.empty())
            return;

        write_in_progress_ = true;

        const auto& buffer = write_queue_.front();

        net::post(
            executor_,
            [this, self{shared_from_this()}, buf = buffer]() mutable {
                handle_ << buf;
                write_in_progress_ = false;
                write_queue_.pop();
                if (!write_queue_.empty())
                    handle_write_queue();
                else
                    cv_.notify_one();
            });
    }

    void ConsoleSink::do_write(std::string&& buffer)
    {
        if (is_stopped_)
            return;

        write_queue_.emplace(std::move(buffer));

        if (write_in_progress_)
            return;

        handle_write_queue();
    }

    void ConsoleSink::close()
    {
        is_stopped_ = true;
        handle_.flush();
    }
}