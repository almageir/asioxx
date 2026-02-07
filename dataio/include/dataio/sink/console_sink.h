#ifndef DATAIO_CONSOLE_SINK_H
#define DATAIO_CONSOLE_SINK_H

#include "sink.h"

#include <asio/any_io_executor.hpp>
#ifdef __linux__
#include <asio/posix/stream_descriptor.hpp>
#endif

#include <queue>
#include <mutex>
#include <condition_variable>

namespace dataio {
#ifdef _WIN32
    using console_handle = std::ostream&;
#else
    using console_handle = asio::posix::stream_descriptor;
#endif

    class ConsoleSink final
        : public Sink
        , public std::enable_shared_from_this<ConsoleSink>
    {
    public:
        ~ConsoleSink() override;

        static std::shared_ptr<ConsoleSink> create(asio::any_io_executor executor);

        ConsoleSink(const ConsoleSink&) = delete;
        ConsoleSink& operator=(const ConsoleSink&) = delete;

        bool write(std::string message) override;

    private:
        std::string buf_;
        explicit ConsoleSink(asio::any_io_executor exec);

        void handle_write_queue();
        void do_write(std::string&& buffer);
        void close();

        asio::any_io_executor executor_;
        console_handle handle_;

        std::queue<std::string> write_queue_;

        std::atomic_bool is_stopped_{false};
        bool write_in_progress_{false};

        std::mutex mutex_;
        std::condition_variable cv_;
    };
}

#endif //DATAIO_CONSOLE_SINK_H