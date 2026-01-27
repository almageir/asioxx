#include <asynclog/global_logger.h>

int main()
{
    namespace  asl = asynclog;

    asl::GlobalLogger::initialize(asl::LogMode::Console | asl::LogMode::File, "trace.log");

    asl::info("Starting", "main()");
    asl::info("Hello World!", "main()");
    asl::info("Finishing", "main()");

    for (std::size_t i = 0; i < 100; ++i)
        asl::debug(std::to_string(i), "for circle");

    return 0;
}
