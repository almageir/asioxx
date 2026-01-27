#ifndef MTLS_TUN_LOG_INTERFACE_H
#define MTLS_TUN_LOG_INTERFACE_H

#include <cstdint>
#include <memory>
#include <string_view>

namespace asynclog {

    enum class MsgType : unsigned
    {
        EDebug,
        EInfo,
        EWarn,
        EError
    };

    enum class LogMode : std::uint8_t {
        None = 0x00,
        File = 0x01,
        Console = 0x02
    };

    constexpr LogMode operator|(LogMode lhs, LogMode rhs) noexcept {
        return static_cast<LogMode>(
            static_cast<std::uint8_t>(lhs) |
            static_cast<std::uint8_t>(rhs));
    }

    constexpr LogMode operator&(LogMode lhs, LogMode rhs) noexcept {
        return static_cast<LogMode>(
            static_cast<std::uint8_t>(lhs) &
            static_cast<std::uint8_t>(rhs));
    }

    class LogManagerInterface {
    public:
        virtual ~LogManagerInterface() = default;
        virtual void open(LogMode mode) = 0;
        virtual void open(LogMode mode, std::string_view path) = 0;
        virtual void write(std::string_view msg, MsgType type, std::string_view source) const = 0;
    };

    using LogManagerPtr = std::shared_ptr<LogManagerInterface>;

}

#endif //MTLS_TUN_LOG_INTERFACE_H