#ifndef ASYNCLOG_COMMON_H
#define ASYNCLOG_COMMON_H

namespace asynclog
{
    enum class MsgType : unsigned
    {
        EDebug,
        EInfo,
        EWarn,
        EError
    };
}

#endif //ASYNCLOG_COMMON_H