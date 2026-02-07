#ifndef DATAIO_FILE_SINK_H
#define DATAIO_FILE_SINK_H

#include "sink.h"

#include <asio/stream_file.hpp>
#include <queue>
#include <mutex>
#include <condition_variable>

#if defined(_WIN32) || (defined __linux__ && defined(ASIO_HAS_IO_URING))

namespace dataio
{
    class FileSink final
        : public Sink
        , public std::enable_shared_from_this<FileSink>
    {
    public:
        explicit FileSink(const asio::any_io_executor& exec, const std::string& path);
        ~FileSink() override;

        bool write(std::string message) override;

        FileSink(const FileSink&) = delete;
        FileSink& operator=(const FileSink&) = delete;

    private:
        void handle_write_queue();
        void do_write(std::string&& buffer);
        void close();

        asio::stream_file file_;
        std::queue<std::string> write_queue_;

        std::atomic_bool is_stopped_{false};
        bool write_in_progress_{false};

        std::mutex mutex_;
        std::condition_variable cv_;
    };
}

#endif

#endif // DATAIO_FILE_SINK_H
