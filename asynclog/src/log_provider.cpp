#include <asynclog/log_provider.h>
#include <asynclog/log_manager.h>

#include <dataio/sink/console_sink.h>
#include <dataio/sink/file_sink.h>

#include <algorithm>
#include <filesystem>
#include <iomanip>
#include <ctime>

namespace
{
    using namespace std::string_literals;
    using namespace std::string_view_literals;

    namespace chr = std::chrono;
    namespace fs = std::filesystem;

    std::string timestamp()
    {
        const auto tp_now = chr::system_clock::now();
        const auto tm_now = chr::system_clock::to_time_t(tp_now);

        const auto millis = chr::duration_cast<chr::milliseconds>(tp_now.time_since_epoch()) % 1000;

        std::tm tm;
#ifdef _WIN32
        localtime_s(&tm, &tm_now);
#else
        localtime_r(&tm_now, &tm);
#endif

        std::ostringstream oss;
        oss << std::put_time(&tm, "%F %T");
        oss << '.' << std::setfill('0') << std::setw(3) << millis.count();

        return oss.str();
    }

    fs::path make_unique_file_name(std::string_view file_path)
    {
        auto ts{timestamp()};

        std::transform(std::cbegin(ts), std::cend(ts), ts.begin(), [](auto c) {
            return (c == ' ' || c == '.') ? '_' : c;
        });

        std::string suffix;
        std::remove_copy_if(std::cbegin(ts), std::cend(ts), std::back_inserter(suffix), [](auto c) {
            return (c == '.' || c == ':' || c == '-') ? true : false;
        });

        const fs::path path{file_path};
        const auto unique_file_name =
            path.parent_path() / (path.stem().string() + "_"s + suffix + path.extension().string());

        return unique_file_name;
    }

    constexpr std::string_view log_level_to_str(asynclog::MsgType idx)
    {
        constexpr std::array<std::string_view, 4> log_desc_map{"dbg"sv, "inf"sv, "wrn"sv, "err"sv};
        return log_desc_map[static_cast<unsigned>(idx)];
    }

    std::string enclose(std::string_view msg, std::string_view prefix, std::string_view suffix)
    {
        std::string result;
        result.reserve(msg.size() + prefix.size() + suffix.size());
        result.append(prefix).append(msg).append(suffix);
        return result;
    }
}

namespace asynclog
{
    LogProvider::LogProvider()
        : log_backend_{std::make_unique<LogManager>()}
    {}

    void LogProvider::open(std::string_view path) const
    {
        SinkPtr fileLogger = std::make_shared<dataio::FileSink>(log_backend_->get_executor(), make_unique_file_name(path).string());
        SinkPtr consoleLogger = std::make_shared<dataio::ConsoleSink>(log_backend_->get_executor());

        log_backend_->addSink(std::move(fileLogger));
        log_backend_->addSink(std::move(consoleLogger));
    }

    void LogProvider::write(std::string_view msg, MsgType type, std::string_view source) const
    {
        enum {
            ETimeStampFieldLength = 24,
            ELogLevelFieldLength = 6,
            ELogSourceFieldLength = 28
        };

        static const std::string spaces(ELogSourceFieldLength, ' ');

        std::ostringstream oss;
        oss << std::left << std::setw(ETimeStampFieldLength) << timestamp();
        oss << std::left << std::setw(ELogLevelFieldLength) << enclose(log_level_to_str(type), "[", "]");
        oss << std::left << std::setw(ELogSourceFieldLength) << (!source.empty() ? enclose(source, "[", "]") : spaces);
        oss << msg << std::endl;

        if (log_backend_)
            log_backend_->push(std::move(oss.str()));
    }
}
