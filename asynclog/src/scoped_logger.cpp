#include <asynclog/scoped_logger.h>

namespace asynclog {

    ScopedLogger::ScopedLogger(std::string_view source, std::shared_ptr<LogManagerInterface> backend)
        : source_{source}
        , backend_{backend}
    {}

    void ScopedLogger::info(std::string_view msg) const {
        if (backend_)
            backend_->write(msg, MsgType::EInfo, source_);
    }

    void ScopedLogger::debug(std::string_view msg) const {
        if (backend_)
            backend_->write(msg, MsgType::EDebug, source_);
    }

    void ScopedLogger::warn(std::string_view msg) const {
        if (backend_)
            backend_->write(msg, MsgType::EWarn, source_);
    }

    void ScopedLogger::err(std::string_view msg) const {
        if (backend_)
            backend_->write(msg, MsgType::EError, source_);
    }

    std::shared_ptr<LogManagerInterface> ScopedLogger::backend() const {
        return backend_;
    }
}
