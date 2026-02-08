#include <dataio/sink/file_sink.h>

#include <asio/write.hpp>

#if defined(_WIN32) || (defined __linux__ && defined(ASIO_HAS_IO_URING))

namespace {
    const asio::file_base::flags open_flags =
        asio::stream_file::create |
        asio::stream_file::write_only |
        asio::stream_file::truncate;
}

namespace dataio
{
    namespace net = asio;

    FileSink::FileSink(const net::any_io_executor& executor, const std::string& path)
        : file_{executor, path, open_flags}
    {}

    FileSink::~FileSink() {
        std::unique_lock lock{mutex_};
        cv_.wait(lock, [this]{ return write_queue_.empty(); });
        is_stopped_ = true;
    }

    std::shared_ptr<FileSink> FileSink::create(asio::any_io_executor executor, const std::string& path) {
        return std::shared_ptr<FileSink>(new FileSink(std::move(executor), path));
    }

    bool FileSink::write(std::string buffer)
    {
        if (buffer.empty())
            return false;

        auto self{shared_from_this()};

        auto fn = [self, buffer = buffer]() mutable {
            self->do_write(std::move(buffer));
        };

        net::post(file_.get_executor(), std::move(fn));

        return true;
    }

    void FileSink::handle_write_queue()
    {
        if (is_stopped_ || !file_.is_open())
            return;

        if (write_queue_.empty())
            return;

        write_in_progress_ = true;

        const auto& buffer = write_queue_.front();

        net::async_write(
            file_, net::buffer(buffer),
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

    void FileSink::do_write(std::string&& buffer)
    {
        if (is_stopped_)
            return;

        write_queue_.emplace(std::move(buffer));

        if (write_in_progress_)
            return;

        handle_write_queue();
    }

    void FileSink::close()
    {
        is_stopped_ = true;
        net::error_code ignored_ec;
        file_.close(ignored_ec);
    }
}

#endif
