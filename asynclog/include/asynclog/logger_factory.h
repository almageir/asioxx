#ifndef MTLS_TUN_LOGGER_FACTORY_H
#define MTLS_TUN_LOGGER_FACTORY_H

#include "scoped_logger.h"

#include <memory>

namespace asynclog {
    class ScopedLogger;
    class LogManagerInterface;

    class LoggerFactory {
    public:
        explicit LoggerFactory(std::shared_ptr<LogManagerInterface> log_backend);
        ~LoggerFactory();

        [[nodiscard]] ScopedLogger create(std::string_view name) const;

    private:
        std::shared_ptr<LogManagerInterface> log_backend_;
    };

}

#endif //MTLS_TUN_LOGGER_FACTORY_H