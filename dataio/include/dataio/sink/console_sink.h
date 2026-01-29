#ifndef DATAIO_CONSOLE_SINK_H
#define DATAIO_CONSOLE_SINK_H

#include "sink.h"

#include <asio/any_io_executor.hpp>

#include <queue>
#include <mutex>
#include <condition_variable>

namespace dataio {
#ifdef _WIN32
    using console_handle = std::ostream&;
#else
    using console_handle = asio::posix::stream_descriptor;
#endif

    class ConsoleSink : public Sink
    {
    public:
        explicit ConsoleSink(asio::any_io_executor exec);
        ~ConsoleSink() override;

        bool write(std::string message) override;

    private:
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