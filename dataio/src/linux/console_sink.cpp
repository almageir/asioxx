#include <iostream>
#include <dataio/sink/console_sink.h>

#include <asio/post.hpp>
#include <asio/write.hpp>

#include <unistd.h>

namespace dataio {

    namespace net = asio;

    ConsoleSink::ConsoleSink(net::any_io_executor executor)
        : executor_{std::move(executor)}
        , handle_{executor_, ::dup(STDOUT_FILENO)}
    {}

    ConsoleSink::~ConsoleSink() {
        std::unique_lock lock{mutex_};
        cv_.wait(lock, [this]{ return write_queue_.empty(); });
        is_stopped_ = true;
    }

    std::shared_ptr<ConsoleSink> ConsoleSink::create(asio::any_io_executor executor) {
        return std::shared_ptr<ConsoleSink>(new ConsoleSink(std::move(executor)));
    }

    bool ConsoleSink::write(std::string buffer)
    {
        if (buffer.empty())
            return false;

        auto self{shared_from_this()};

        auto fn = [self, buffer = buffer]() mutable {
            self->do_write(std::move(buffer));
        };

        net::post(executor_, std::move(fn));

        return true;
    }

    void ConsoleSink::handle_write_queue()
    {
        if (is_stopped_ || !handle_.is_open())
            return;

        if (write_queue_.empty())
            return;

        write_in_progress_ = true;
        const auto& buffer = write_queue_.front();

        net::async_write(
            handle_, net::buffer(buffer),
            [this, self{shared_from_this()}](const net::error_code& ec, std::size_t bytes_transferred) {
                write_in_progress_ = false;
                if (!ec) {
                    write_queue_.pop();
                    if (!write_queue_.empty())
                        handle_write_queue();
                    else
                        cv_.notify_all();
                } else {
                    close();
                }
            });
    }

    void ConsoleSink::do_write(std::string&& buffer)
    {
        if (is_stopped_)
            return;

        write_queue_.push(std::move(buffer));

        if (write_in_progress_)
            return;

        handle_write_queue();
    }

    void ConsoleSink::close()
    {
        is_stopped_ = true;
        net::error_code ignored_ec;
        handle_.close(ignored_ec);
    }

}