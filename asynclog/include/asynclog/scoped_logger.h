#ifndef MTLS_TUN_SCOPED_LOGGER_H
#define MTLS_TUN_SCOPED_LOGGER_H

#include "log_manager_interface.h"

#include <memory>
#include <string>
#include <string_view>

namespace asynclog {

    class ScopedLogger {
    public:
        ScopedLogger(std::string_view source, std::shared_ptr<LogManagerInterface> backend);

        ScopedLogger(const ScopedLogger&) = delete;
        ScopedLogger& operator=(const ScopedLogger&) = delete;
        ScopedLogger(ScopedLogger&&) = default;
        ScopedLogger& operator=(ScopedLogger&&) = default;

        void info(std::string_view msg) const;
        void debug(std::string_view msg) const;
        void warn(std::string_view msg) const;
        void err(std::string_view msg) const;

        std::shared_ptr<LogManagerInterface> backend() const;

    private:
        std::string source_;
        std::shared_ptr<LogManagerInterface> backend_;
    };
}

#endif //MTLS_TUN_SCOPED_LOGGER_H